/**
 * @fileOverview AKP Analytics Javascript SDK
 * @author Gonzalo Rubio <gonzalo.rubio@autodesk.com>
 */

/**
 * @namespace
 * @see https://beehive.jira.com/wiki/display/BEEHIVE/AKP+Analytics+Integration+Services
 */

window.AKPAnalytics = window.AKPAnalytics || {};

;(function($, window, document, undefined){

    var DEFAULT_BDP_SERVER = '//beehive-stage.autodesk.com';
    var DEFAULT_ADP_SERVER = '//akn.analytics-staging.autodesk.com';
    var BDP_LOG_ENDPOINT = '/akpanalyticsservices/api/eventlog/v1/app/{{appId}}/message';
    var BDP_TRACK_ENDPOINT = '/akpeventservices/api/eventtrack/v1/event/data';
    var ADP_ENDPOINT = '/analyticsservices/api/event/v1/app/{{appId}}/message';
    var ADP_TRACK_ENDPOINT = '/akpeventservices/api/eventtrack/v2/app/{{appId}}/event/data';
    var AUTH_TOKEN_TEMPLATE = 'Akn {{apiKey}}:{{signature}}';

    var _util = {
        // tiny mustache-like helper for text interpolation
        interpolateText: function(template, data) {
            return template.replace(/(?:\{\{)(\w+?)(?:\}\})/g, function(m, p1){
                return data[p1] || "";
            }).replace(/(?:\{\{)([\w\.]+?)(?:\}\})/g, function(m, p1){
                var path = p1.split('.');
                return data[path[0]][path[1]] || "";
            });
        },

        // RFC-4122 compliant guid creation
        createGuid: function() {
            return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
                var r = Math.random()*16|0, v = c === 'x' ? r : (r&0x3|0x8);
                return v.toString(16);
            });
        },

        sign: function(message, key) {
            if ('CryptoJS' in window && window.CryptoJS.HmacSHA256 && window.CryptoJS.enc.Base64) {
                return CryptoJS.enc.Base64.stringify(CryptoJS.HmacSHA256(message, key));
            } else if ('jsSHA' in window) {
                var shaObj = new jsSHA("SHA-256", "TEXT");

                if (shaObj.setHMACKey) { // check if it's jsSHA 2.x+
                    shaObj.setHMACKey(key, "TEXT");
                    shaObj.update(message);
                    return shaObj.getHMAC("B64");
                } else { // jsSHA 1.x API
                    shaObj = new jsSHA(message, "ASCII");
                    return shaObj.getHMAC(key, "ASCII", "SHA-256", "B64") + "=";
                }
            } else {
                throw "No SHA-256 signing library detected or provided. Supported: CryptoJS, jsSHA or your own via signWith option.";
            }
        },

        callBDP: function(server, appId, endpointTemplate, data) {
            return $.ajax({
                url: server + _util.interpolateText(endpointTemplate, {appId: appId}),
                dataType: 'jsonp',
                scriptCharset: "utf-8",
                data: data || {}
            }).then(function(data) {
                if (data.MESSAGE === 'SUCCESS') {
                    return data;
                } else { // any response different than MESSAGE: "SUCCESS" will fail the request
                    return $.Deferred().reject();
                }
            });
        },

        callADP: (function() {
            var _sign = function(message, secretORsigner) {
                if (typeof secretORsigner === 'function') {
                    return secretORsigner(message);
                } else {
                    return _util.sign(message, secretORsigner);
                }
            };

            return function(server, appId, apiKey, secretORsigner, endpointTemplate, data) {
                var timestamp = (new Date()).toISOString();
                var event = data.event || data.e;

                return $.when(_sign(apiKey + timestamp + event, secretORsigner)).then(function(signature) {
                    return $.ajax({
                        type: "POST",
                        url: server + _util.interpolateText(endpointTemplate, {appId: appId}),
                        dataType: "json",
                        contentType: "application/json;charset=utf-8",
                        headers: {
                            "Authorization": _util.interpolateText(AUTH_TOKEN_TEMPLATE, {
                                apiKey: apiKey,
                                signature: signature
                            })
                        },
                        data: JSON.stringify($.extend({time: timestamp}, data || {}))
                    });
                }).then(function(data) {
                    if (data.MESSAGE === 'SUCCESS') {
                        return data;
                    } else { // any response different than MESSAGE: "SUCCESS" will fail the request
                        return $.Deferred().reject();
                    }
                });
            };
        })()
    };

    /**
     * @private
     * @constructor
     * @classdesc DataStore abstraction for localStorage and sessionStorage that saves and retrieves values as JSON
     * @param {string} [mode=sessionStorage]
     */
    var DataStore = (function() {
        var HAS_DOM_STORAGE = ('sessionStorage' in window) && ('localStorage' in window) && (window.sessionStorage) && (typeof window.sessionStorage.getItem === 'function') && ('JSON' in window);

        var _storage = {
            getItem: function(from, key) {
                if (!HAS_DOM_STORAGE || !window[from]) return;

                var value = window[from].getItem(key);

                try {
                    return JSON.parse(value);
                } catch (e) {
                    return value;
                }
            },

            setItem: function(to, key, value) {
                if (!HAS_DOM_STORAGE || !window[to]) return;

                try {
                    // private mode Safari can throw here
                    window[to].setItem(key, JSON.stringify(value));
                } catch (e) {

                }
            },

            removeItem: function(from, key) {
                if (!HAS_DOM_STORAGE || !window[from]) return;

                window[from].removeItem(key);
            }
        };

        return function(mode) {
            // anything that doesn't match localStorage will default to sessionStorage
            if (mode !== 'localStorage') mode = 'sessionStorage';
            this._mode = mode;

            for (var key in _storage) {
                if (!_storage.hasOwnProperty(key)) continue;
                this[key] = $.proxy(_storage[key], this, this._mode);
            }
        };
    })();

    /**
     * @private
     * @param {string} key
     */
    var readCookie = function(key) {
        var cookies = document.cookie.split('; ');
        var result;

        for (var i = 0; i < cookies.length; i++) {
            var parts = cookies[i].split('=');

            var name = decodeURIComponent(parts[0]);
            var cookie = decodeURIComponent(parts[1]);

            if (key && key === name) {
                result = cookie;
                break;
            }
        }

        return result;
    };

    /**
     * @memberOf AKPAnalytics
     * @constructor
     * @classdesc Logger Contains the raw log call needed for interacting with the AKP Analytics service
     * @param {string} appId Application id for the app which will be sending data for analysis (required)
     * @param {string} [server=//beehive-stage.autodesk.com] Base URL to the server containing the AKP Analytics platform
     * @param {Object} [ADPData] If passed, it sets the required params for sending requests against the ADP endpoint
     *   @param {string} ADPData.apiKey API key set for the appId identifier
     *   @param {string|function} ADPData.signWith the secret used for signing the ADP authorization header, or a function that's used for signing. If it's a function, it can optionally return a Deferred to asynchronously sign messages.
     */
    var Logger = function(appId, server, ADPData) {
        if (!appId) throw new Error('You need to pass an appId to instantiate a logger.');

        this._appId = appId;
        this._server = server || (ADPData ? DEFAULT_ADP_SERVER : DEFAULT_BDP_SERVER);
        this._ADPData = ADPData;
    };

    /**
     * Logging helper to call AKP analytics server using the eventlog endpoint
     * @param {Object} data Raw data object with all the params to be logged to the server
     * @return {Deferred}
     */
    Logger.prototype.log = function(data) {
        if (this._ADPData) {
            return _util.callADP(this._server, this._appId, this._ADPData.apiKey, this._ADPData.signWith || this._ADPData.secret, ADP_ENDPOINT, data);
        } else {
            return _util.callBDP(this._server, this._appId, BDP_LOG_ENDPOINT, data);
        }
    };

    /**
     * Logging helper to call AKP analytics server using the eventtrack endpoint
     * @param {Object} data Raw data object with all the params to be logged to the server
     * @return {Deferred}
     */
    Logger.prototype.track = function(data) {
        if (this._ADPData) {
            return _util.callADP(this._server, this._appId, this._ADPData.apiKey, this._ADPData.signWith || this._ADPData.secret, ADP_TRACK_ENDPOINT, data);
        } else {
            return _util.callBDP(this._server, this._appId, BDP_TRACK_ENDPOINT, data);
        }
    };

    /**
     * @memberOf AKPAnalytics
     * @constructor
     * @classdesc LoggingHelper encapsulates basic logging capabilities with the possibility of keeping state for this and future sessions
     * @param {Object} options The options object to customize the helper.
     *   @param {string} options.appId required, application id for the invoking app.
     *   @param {string} options.event required, event that will be logged to the server.
     *   @param {string} [options.ADPData] optional, configuration for logging data onto ADP, defined in Logger
     *   @param {string} [options.server] optional, server to use for logging
     *   @param {string} [options.userId] optional, can be used to pre-set a user identifier and overrides the 'ui' param in the data object
     *   @param {string} [options.userSessionId] optional, can be used to pre-set a session-based user identifier and overrides the 'usi' param in the data object
     *   @param {function} [options.onStateChange] callback fired every time a new value is set, accepts returning a deferred for async operations
     *   @param {string|boolean} [options.stateful] allows saving the last logged value in HTML5's sessionStorage or localStorage
     *   @param {Object} options.data data to be sent to the server, as per https://beehive.jira.com/wiki/display/BEEHIVE/AKP+Analytics+Integration+Services
     */
    var LoggingHelper = function(options) {
        var defaultOptions = {
            appId: '',
            onStateChange: function() {}
        };

        this.options = $.extend({}, defaultOptions, options);
        this.options.data = this.options.data || {};
        this._value = undefined;

        if (!this.options.appId) throw new Error('You need an appId to instantiate the helper');
        if (!this.options.event) throw new Error('You need to pass an event to be logged on the server');

        this._logger = new Logger(this.options.appId, this.options.server, this.options.ADPData);
        this._contentId = encodeURIComponent(this.options.appId + '-' + this.options.event + '-' + window.location.pathname + window.location.search + window.location.hash);

        if (this.options.stateful) {
            this._storageId = 'akp-analytics-' + this.options.event;
            this._dataStore = new DataStore(this.options.stateful);
        }

        this.refresh();
    };

    /**
     * Refreshes the state of the logger, useful for stateful loggers.
     * @return {this}
     */
    LoggingHelper.prototype.refresh = function() {
        // read userId from storage, only use if it hasn't been passed in as options
        var userStore = new DataStore('localStorage');
        this._userId = this.options.userId || this._userId || userStore.getItem('akp-analytics-userid');

        if (!this._userId) {
            this._userId = _util.createGuid();
            userStore.setItem('akp-analytics-userid', this._userId);
        }

        // read userSessionId from storage, same as above
        var userSessionStore = new DataStore('sessionStorage');
        this._userSessionId = this.options.userSessionId || this._userSessionId || userSessionStore.getItem('akp-analytics-userid');

        if (!this._userSessionId) {
            this._userSessionId = _util.createGuid();
            userSessionStore.setItem('akp-analytics-userid', this._userSessionId);
        }

        // read content state if any
        if (this.options.stateful) {
            var values = this._dataStore.getItem(this._storageId) || {};

            this._value = values[this._contentId];
        }

        return this;
    };

    /**
     * Returns the data that is used for logging to the server, including the current value of the instance
     * This function mixes-in all of the required event data for comprehensive analytics.
     * @return {Object}
     */
    LoggingHelper.prototype.getData = function() {
        return $.extend({}, {
            ui: this._userId,
            usi: this._userSessionId,
            ga: readCookie('_ga'),
            c: readCookie('excntry'),
            bl: window.navigator.language || window.navigator.browserLanguage || undefined,
            t: document.title,
            u: window.location.href,
            ru: document.referrer || undefined,
            e: this.options.event,
            ev: this._value
        }, this.options.data);
    };

    /**
     * Updates the value of the base data that is sent to the server, overrides existing keys and augments with new values
     * @param {Object} data
     * @return {this}
     */
    LoggingHelper.prototype.setData = function(data) {
        $.extend(this.options.data, data || {});
        return this;
    };

    /**
     * Returns the last value set on the logger. If this is a stateful logger, the initial value will be persisted from the last session.
     * @return {*}
     */
    LoggingHelper.prototype.getValue = function() {
        return this._value;
    };

    /**
     * Sets a new value for the logger and calls the AKP analytics server with the new data.
     * @param {*} value Value to be sent to the server, associated with the event that we are logging
     * @param {Object} [dataOverride] optionally allows to send arbitrary data, overriding any pre-set value for the instance
     * @fires AKPAnalytics.LoggingHelper#onStateChange
     * @return {Deferred}
     */
    LoggingHelper.prototype.setValue = function(value, dataOverride) {
        var self = this;
        var data = $.extend(this.getData(), dataOverride || {});

        // onStateChange can return a different value and data params, cancel the operation or simply be used to delay the call until something else happens
        return $.when(this.options.onStateChange && this.options.onStateChange(value, data)).then(function(args) {
            if (args !== false) {
                if (Array.isArray(args)) {
                    // returning an array allows overriding event value and data
                    self._value = args[0];
                    if (typeof args[1] === 'object') data = args[1];
                } else {
                    self._value = value;
                }

                data.ev = self._value;

                return $.Deferred().resolve(self._value);
            } else {
                return $.Deferred().reject();
            }
        }).then(function(val) {
            // don't create a request if value is undefined
            var request = (val === undefined) ? $.Deferred().resolve() : self._logger.log(data);

            return request.then(function() {
                if (self.options.stateful) {
                    var values = self._dataStore.getItem(self._storageId) || {};

                    if (val === undefined) {
                        delete values[self._contentId];
                    } else {
                        values[self._contentId] = val;
                    }

                    self._dataStore.setItem(self._storageId, values);
                }

                return val;
            });
        });
    };

    // external API
    $.extend(window.AKPAnalytics, {
        /**
         * Raw logging interface as a static method provided for convenience
         * @static
         * @param {string} appId
         * @param {string} server
         * @param {string} [ADPData]
         * @param {Object} data
         */
        log: function(appId, server, ADPData, data) {
            var logger = new Logger(appId, server, ADPData);

            return logger.log(data);
        },

        track: function(appId, server, ADPData, data) {
            var logger = new Logger(appId, server, ADPData);

            return logger.track(data);
        },

        /**
         * Raw logging interface that automatically includes session identifiers and basic tracking params on the data
         * @static
         * @param {string} appId
         * @param {string} server
         * @param {string} [ADPData]
         * @param {string} event
         * @param {string} value
         * @param {Object} data
         */
        logWithTracking: function(appId, server, ADPData, event, value, data) {
            var logger = new AKPAnalytics.LoggingHelper({
                appId: appId,
                server: server,
                ADPData: ADPData,
                stateful: false,
                event: event,
                data: data || {}
            });

            return logger.setValue(value);
        },

        Logger: Logger,
        LoggingHelper: LoggingHelper,
        util: _util
    });

})(jQuery, window, document);
