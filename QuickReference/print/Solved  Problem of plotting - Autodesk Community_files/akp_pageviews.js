/**
 * @fileOverview AKP Analytics Page Views Plugin
 * @author Gonzalo Rubio <gonzalo.rubio@autodesk.com>
 */

;(function($, window, document, undefined) {

    var _getScrollParent = function(element) {
        if (element instanceof $) element = element[0];

        var ownerDocument = element.ownerDocument;
        var getComputedStyle = ownerDocument.defaultView.getComputedStyle;
        var node = element.parentNode;
        var style, overflow, overflowX, overflowY;

        while (node && (node !== ownerDocument)) {
            style = getComputedStyle(node);
            overflow = style.getPropertyValue('overflow');
            overflowX = style.getPropertyValue('overflow-x');
            overflowY = style.getPropertyValue('overflow-y');

            if (!overflow && (overflowX !== overflowY) && (overflowY === 'scroll')) {
                overflow = overflowY;
            }

            if ((overflow === 'auto') && (overflowX === 'hidden')) {
                overflow = '';
            }

            if (/(auto|scroll)/.test(overflow)) {
                break;
            } else  {
                node = node.parentNode;
            }
        }

        if (node === ownerDocument.documentElement) {
            node = ownerDocument;
        }

        return node || element;
    };

    var AKPPageViews = function(element, options) {
        this.options = $.extend(true, {}, this.defaultOptions, options || {});
        this.element = $(element);
        this._scrollCallback = this._scrollCallback.bind(this);

        this._create();
    };

    $.extend(AKPPageViews.prototype, {
        defaultOptions: {
            server: "//beehive-stage.autodesk.com",
            events: {
                PAGE_VIEW: "page_view",
                START_READING: "page_view_reading",
                CONTENT_BOTTOM: "page_view_content_bottom",
                PAGE_BOTTOM: "page_view_page_bottom",
                TRACK_START_READING: "view"
            },
            useTrackEvent: true,
            userTypes: {
                READER: "reader",
                SCANNER: "scanner"
            },
            lengthTypes: {
                NORMAL: "normal",
                SHORT: "short"
            },
            READING_DELTA: 150,
            READER_TIME: 60000,
            SHORT_ARTICLE_READER_TIME: 30000,
            onSendEvent: function() {}
        },

        _create: function() {
            var ownerDocument = this.element[0].ownerDocument;

            this._sessionStart = Date.now();
            this._startReadingSent = false;
            this._contentBottomSent = false;
            this._pageBottomSent = false;

            this.scrollElement = this.options.scrollParent || $(_getScrollParent(this.element));

            this._useScrollEvent = !!this._getScrollSize(this.scrollElement[0]);
            this.options.data.cltype = this._useScrollEvent ? this.options.lengthTypes.NORMAL : this.options.lengthTypes.SHORT;

            if (this._useScrollEvent) {
                this.scrollElement.on("scroll", this._scrollCallback);
            } else {
                this.readTimer = setTimeout(function() {
                    if (!this.element.is(':visible')) return;

                    this._sendEvent(this.options.events.START_READING, ownerDocument.location.href);

                    if (this.options.useTrackEvent) {
                        this._sendTrack(this.options.events.TRACK_START_READING, ownerDocument.location.href);
                    }
                }.bind(this), this.options.SHORT_ARTICLE_READER_TIME);
            }

            this._sendData = AKPAnalytics.logWithTracking.bind(AKPAnalytics, this.options.appId, this.options.server, this.options.ADPData);
            this._sendEvent(this.options.events.PAGE_VIEW, ownerDocument.location.href);

            return this;
        },

        _destroy: function () {
            if (this._useScrollEvent) {
                this.scrollElement.off("scroll", this._scrollCallback);
            } else {
                if (this.readTimer) {
                    clearTimeout(this.readTimer);
                    this.readTimer = null;
                }
            }
            this.element.removeData("autodeskAKPPageViews");

            return this;
        },

        _getScrollSize: function(element) {
            if (!element.ownerDocument && element.documentElement && (element.defaultView.document === element)) {
                return element.documentElement.scrollHeight - element.documentElement.clientHeight;
            } else {
                return element.scrollHeight - element.clientHeight;
            }
        },

        _getScrollTop: function(element) {
            if ('scrollTop' in element) {
                return element.scrollTop;
            } else if (element.defaultView && element.documentElement) {
                return element.defaultView.scrollY || element.documentElement.scrollTop;
            } else {
                // give up
                return this.element[0].ownerDocument.defaultView.pageYOffset || 0;
            }
        },

        _scrollCallback: function(e) {
            if (!this.element.is(':visible')) return;

            var timeDelta = Date.now() - this._sessionStart;
            var distanceDelta = this._getScrollTop(this.scrollElement[0]);
            var scrollSize = this._getScrollSize(this.scrollElement[0]);
            var bounds = this.element[0].getBoundingClientRect();
            var readingDelta = (scrollSize > this.options.READING_DELTA) ? this.options.READING_DELTA : scrollSize * 0.75;
            var ownerDocument = this.element[0].ownerDocument;

            if (!this._startReadingSent && (distanceDelta >= readingDelta)) {
                this._sendEvent(this.options.events.START_READING, ownerDocument.location.href);

                if (this.options.useTrackEvent) {
                    this._sendTrack(this.options.events.TRACK_START_READING, ownerDocument.location.href);
                }

                this._startReadingSent = true;
            }

            if (!this._contentBottomSent && (ownerDocument.defaultView.innerHeight >= Math.floor(bounds.bottom))) {
                this._sendEvent(this.options.events.CONTENT_BOTTOM, ownerDocument.location.href, {
                    ts: Math.round(timeDelta / 1000),
                    eut: timeDelta > this.options.READER_TIME ? this.options.userTypes.READER : this.options.userTypes.SCANNER
                });
                this._contentBottomSent = true;
            }

            if (!this._pageBottomSent && (distanceDelta >= Math.floor(scrollSize))) {
                this._sendEvent(this.options.events.PAGE_BOTTOM, ownerDocument.location.href);
                this._pageBottomSent = true;
            }
        },

        _sendEvent: function(event, value, data) {
            data = data || {};

            var eventData = $.extend({}, this.options.data, data);

            $.when(this.options.onSendEvent(event, value, eventData)).then(function(args) {
                if (Array.isArray(args)) {
                    this._sendData.apply(this, args);
                } else if (args !== false) {
                    this._sendData(event, value, eventData);
                }
            }.bind(this));

            return this;
        },

        _sendTrack: function(event, value) {
            var helper = new AKPAnalytics.LoggingHelper({
                appId: this.options.appId,
                server: this.options.server,
                ADPData: this.options.ADPData,
                event: event,
                stateful: false,
                data: this.options.data
            });

            var helperData = helper.getData();

            var eventData = {
                eventKey: helperData.ck || helperData.u || value,
                keyType: !!helperData.ck ? 'caaskey' : 'url',
                event: event,
                eventValue: true,
                source: helperData.s,
                language: helperData.l,
                userSessionId: helperData.usi
            };

            helper._logger.track(eventData);
        },

        setOptions: function(options) {
            $.extend(true, this.options, options);

            if (options.server || options.events) {
                this._destroy();
                this._create();
            }

            return this;
        }
    });

    $.fn.akp_pageviews = function(options) {
        var widget = this.data("autodeskAKPPageViews");

        if (widget) {
            switch (options) {
            case "option":
                return widget.setOptions(arguments[1]);
            case "destroy":
                return widget._destroy();
            default:
                return widget;
            }
        } else {
            widget = new AKPPageViews(this, options);

            this.data("autodeskAKPPageViews", widget);
            return widget;
        }
    };

    // register on AKPAnalytics as a plugin
    window.AKPAnalytics = window.AKPAnalytics || {};
    window.AKPAnalytics.plugins = window.AKPAnalytics.plugins || {};
    window.AKPAnalytics.plugins.AKPPageViews = AKPPageViews;

})(jQuery, window, document);
