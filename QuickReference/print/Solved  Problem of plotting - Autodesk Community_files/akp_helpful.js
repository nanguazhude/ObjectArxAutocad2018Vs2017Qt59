/**
 * @fileOverview AKP Analytics "Was this Helpful" Plugin
 * @author Gonzalo Rubio <gonzalo.rubio@autodesk.com>
 */

;(function($, window, document, undefined) {

    var AKPHelpful = function(element, options) {
        this._instanceid = "wtah-" + Date.now();

        var $element = $(element);

        if ($element.hasClass('was-this-helpful')) {
            this.element = $element;
        } else {
            this.element = $(document.createElement('div')).addClass('was-this-helpful');
            $element.append(this.element);
        }

        this.setOptions($.extend(true, {}, this.defaultOptions, options || {}));
    };

    $.extend(AKPHelpful.prototype, {
        defaultOptions: {
            server: '//beehive-stage.autodesk.com',
            event: 'helpful',
            feedback_event: 'helpful_comment',
            useTrackEvent: true,
            lang: "ENU",
            cssClasses: {
                base: 'helpful-container',
                mainActions: 'helpful-actions',
                statusBar: 'helpful-status',
                feedbackForm: 'helpful-feedback',
                commentForm: 'helpful-comment-form',
                commentCounter: 'helpful-comment-counter',
                commentSubmitButton: 'helpful-button',
                ctaSectionContainer: 'helpful-cta-container',
                ctaSection: 'helpful-cta',
                ctaButton: 'helpful-button',
                ctaPanel: 'helpful-cta-panel'
            },
            settings: {
                stateful: 'localStorage',
                commentsEnabled: true,
                commentsMaxLength: 300,
                callToAction: {
                    'true': {
                        show: false,
                        after: 'value',
                        templateName: 'callToActionYes'
                    },
                    'false': {
                        show: false,
                        after: 'feedback',
                        templateName: 'callToActionNo'
                    }
                }
            },
            templates: {
                main: '<div class="{{cssClasses.base}}" lang="{{settings.languageISOcode}}">' +
                            '<h3 id="title-{{instanceid}}">{{i18n.HELPFUL_HEADER}}</h3>' +
                            '<ul role="radiogroup" aria-labelledby="title-{{instanceid}}" class="{{cssClasses.mainActions}}">' +
                                '<li><span role="radio" aria-checked="false" data-value="true" class="helpful-radio" aria-labelledby="label-{{instanceid}}-yes" tabindex="0"></span><label id="label-{{instanceid}}-yes">{{i18n.HELPFUL_YES}}</label></li>' +
                                '<li><span role="radio" aria-checked="false" data-value="false" class="helpful-radio" aria-labelledby="label-{{instanceid}}-no" tabindex="0"></span><label id="label-{{instanceid}}-no">{{i18n.HELPFUL_NO}}</label></li>' +
                            '</ul>' +
                            '<div class="{{cssClasses.statusBar}}" aria-hidden="true"></div>' +
                            '<form class="{{cssClasses.feedbackForm}}" aria-hidden="true"></form>' +
                            '<div class="{{cssClasses.ctaSectionContainer}}" aria-hidden="true"></div>' +
                        '</div>',
                feedbackYes: '{{commentForm}}' +
                             '<button type="submit" class="{{cssClasses.commentSubmitButton}}" aria-disabled="true">{{i18n.HELPFUL_COMMENT_SUBMIT}}</button>',
                feedbackNo: '<p class="{{cssClasses.feedbackForm}}-title" id="title-{{instanceid}}-nf">{{i18n.HELPFUL_NEGATIVE_TITLE}}</p>' +
                            '<ul role="radiogroup" aria-labelledby="title-{{instanceid}}-nf">' +
                                '<li><span role="radio" aria-checked="false" data-value="Quality" data-allow-feedback="true" class="helpful-radio" aria-labelledby="label-{{instanceid}}-quality" tabindex="0"></span><label id="label-{{instanceid}}-quality">{{i18n.HELPFUL_NEGATIVE_REASON_QUALITY}}</label></li>' +
                                '<li><span role="radio" aria-checked="false" data-value="Findability" data-allow-feedback="true" class="helpful-radio" aria-labelledby="label-{{instanceid}}-findability" tabindex="0"></span><label id="label-{{instanceid}}-findability">{{i18n.HELPFUL_NEGATIVE_REASON_FINDABILITY}}</label></li>' +
                                '<li><span role="radio" aria-checked="false" data-value="Localization" class="helpful-radio" aria-labelledby="label-{{instanceid}}-localization" tabindex="0"></span><label id="label-{{instanceid}}-localization">{{i18n.HELPFUL_NEGATIVE_REASON_LOCALIZATION}}</label></li>' +
                                '<li><span role="radio" aria-checked="false" data-value="Policy" class="helpful-radio" aria-labelledby="label-{{instanceid}}-policy" tabindex="0"></span><label id="label-{{instanceid}}-policy">{{i18n.HELPFUL_NEGATIVE_REASON_POLICY}}</label></li>' +
                                '<li><span role="radio" aria-checked="false" data-value="Other" data-allow-feedback="true" class="helpful-radio" aria-labelledby="label-{{instanceid}}-other" tabindex="0"></span><label id="label-{{instanceid}}-other">{{i18n.HELPFUL_NEGATIVE_REASON_OTHER}}</label></li>' +
                            '</ul>' +
                            '<button type="submit" class="{{cssClasses.commentSubmitButton}}" aria-disabled="true">{{i18n.HELPFUL_COMMENT_SUBMIT}}</button>',
                commentForm: '<div class="{{cssClasses.commentForm}}">' +
                                '<p>{{feedbackPrompt}}</p>' +
                                '<textarea class="helpful-comment-field" maxlength="{{settings.commentsMaxLength}}" placeholder="{{i18n.HELPFUL_COMMENT_PLACEHOLDER_TEXT}}"></textarea>' +
                                '<span class="{{cssClasses.commentCounter}}">0/{{settings.commentsMaxLength}}</span>' +
                            '</div>',
                callToActionYes: '',
                callToActionNo: '<div role="complementary" class="{{cssClasses.ctaSection}}">' +
                                    '<h3>{{i18n.HELPFUL_CTA_SECTION_TITLE}}</h3>' +
                                    '<div class="{{cssClasses.ctaPanel}}-container">' +
                                    '<div class="{{cssClasses.ctaPanel}} helpful-cta-forums">' +
                                        '<h4 class="{{cssClasses.ctaPanel}}-title">{{i18n.HELPFUL_CTA_SECTION_FORUMS_TITLE}}</h4>' +
                                        '<p class="{{cssClasses.ctaPanel}}-body">{{i18n.HELPFUL_CTA_SECTION_FORUMS_TEXT}}</p>' +
                                        '<div class="{{cssClasses.ctaPanel}}-action"><a class="{{cssClasses.ctaButton}}" href="{{i18n.HELPFUL_CTA_SECTION_FORUMS_URL}}" target="_blank">{{i18n.HELPFUL_CTA_SECTION_FORUMS_BUTTON}}</a></div>' +
                                    '</div>' +
                                    '<div class="{{cssClasses.ctaPanel}} helpful-cta-support">' +
                                        '<h4 class="{{cssClasses.ctaPanel}}-title">{{i18n.HELPFUL_CTA_SECTION_SUPPORT_TITLE}}</h4>' +
                                        '<p class="{{cssClasses.ctaPanel}}-body">{{i18n.HELPFUL_CTA_SECTION_SUPPORT_TEXT}}</p>' +
                                        '<div class="{{cssClasses.ctaPanel}}-action"><a class="{{cssClasses.ctaButton}}" href="{{i18n.HELPFUL_CTA_SECTION_SUPPORT_URL}}" target="_blank">{{i18n.HELPFUL_CTA_SECTION_SUPPORT_BUTTON}}</a></div>' +
                                    '</div>' +
                                    '</div>' +
                                '</div>'
            }
        },

        setOptions: function(options) {
            var _initializing = !this.options;
            var _requiresCreate = false;

            if (_initializing) {
                this.options = options;
                _requiresCreate = true;
            } else {
                $.extend(true, this.options, options);
            }

            if (_initializing || options.server || options.event || options.feedback_event || options.data) {
                // initiate a logging helper
                this.loggingHelper = new AKPAnalytics.LoggingHelper({
                    appId: this.options.appId,
                    server: this.options.server,
                    userId: this.options.userId,
                    event: this.options.event,
                    stateful: this.options.settings.stateful,
                    data: this.options.data,
                    onStateChange: $.proxy(this._onStateChange, this)
                });
            }

            if (_initializing || options.i18n || options.lang) {
                _requiresCreate = true;

                if (options.lang && ((options.lang.length !== 3) || (options.lang.toUpperCase() !== options.lang))) {
                    // assume we got an ISO language insted of UPI
                    this.options.lang = AKPHelpful._UPIfromISO(options.lang);
                }

                this._i18n = $.extend({}, AKPHelpful.i18n.ENU, AKPHelpful.i18n[this.options.lang], this.options.i18n || {});

                this.options.settings.languageISOcode = options.settings.languageISOcode || AKPHelpful._languageCodes[this.options.lang];
            }

            if (_initializing || options.cssClasses) {
                _requiresCreate = true;
                this._selectors = {};

                for (var c in this.options.cssClasses) {
                    if (this.options.cssClasses.hasOwnProperty(c)) {
                        this._selectors[c] = '.' + this.options.cssClasses[c].split(' ').join('.');
                    }
                }
            }

            if (_requiresCreate) {
                if (!_initializing) this._destroy();

                this._create();
            } else {
                this.refresh();
            }

            return this;
        },

        clearSelection: function() {
            return this.setValue(undefined);
        },

        setValue: function(value) {
            // setValue automatically calls eventlog
            return this.loggingHelper.setValue(value).then(function(val) {
                // optional call to eventtrack
                if ((val !== undefined) && this.options.useTrackEvent) {
                    var data = this.loggingHelper.getData();

                    $.extend(data, {
                        ekey: data.ck || data.u,
                        kt: !!data.ck ? 'caaskey' : 'url'
                    });

                    this.loggingHelper._logger.track(data);
                }
            }.bind(this));
        },

        renderPartial: function(name, where, options) {
            if (this.options.templates[name]) {
                where.append(this._renderTemplate(name, options));
                this.applyBehavior(name, where);
            }
        },

        applyBehavior: function(name, where) {
            if (this._renderBehaviors[name]) {
                this._renderBehaviors[name].call(this, where);
            }
        },

        _onStateChange: function(value, data) {
            return $.when(this.options.onStateChange ? this.options.onStateChange(value, data) : [value, data]).then(function(args) {
                if (Array.isArray(args)) value = args[0];
                if (value !== undefined) this.__userStateChanged = true;

                this.refresh(value);

                return args;
            }.bind(this));
        },

        _disableButtons: function() {
            this.element.find(this._selectors.mainActions).attr('aria-hidden', 'true').find('[role="radio"]').attr('aria-disabled', 'true');
        },

        _enableButtons: function() {
            this.element.find(this._selectors.mainActions).attr('aria-hidden', 'false').find('[role="radio"]').attr('aria-disabled', 'false');
        },

        _updateSubmitState: function(form) {
            var valid = this._validateForm(form);
            var button = form.find('button[type="submit"]');

            button.attr({
                'aria-disabled': !valid,
                tabindex: valid ? 0 : -1
            });
        },

        _limitLength: function(textarea) {
            if ('maxLength' in textarea[0]) return textarea;

            var fn = {
                commonFilter: function (e) {
                    var maxlen = parseInt(textarea.attr('maxlength'), 10) || 300;
                    var val = $.trim(textarea.val());

                    if (val.length > maxlen) {
                        textarea.val(val.substring(0, maxlen));
                        e.preventDefault();
                    }
                },
                downFilter: function (e) {
                    var maxlen = parseInt(textarea.attr('maxlength'), 10) || 300;
                    var val = $.trim(textarea.val());

                    if (val.length === maxlen && e.which > 32) e.preventDefault();
                }
            };

            return textarea.on({ keypress: fn.commonFilter, keyup: fn.commonFilter, blur: fn.commonFilter, keydown: fn.downFilter });
        },

        _renderBehaviors: {
            main: function(element) {
                this._bindHandler('radioButtons', element.find(this._selectors.mainActions), this.setValue.bind(this));
                this._bindHandler('form', element.find(this._selectors.base));
            },

            feedbackYes: function(element) {
                this.applyBehavior('commentForm', element);
            },

            feedbackNo: function(element) {
                var self = this;

                this._bindHandler('radioButtons', element.find('[role="radiogroup"]'), function(value) {
                    var allowFeedback = this.getAttribute('data-allow-feedback') === 'true';
                    var parent = $(this).parent();

                    if (allowFeedback && !parent.find(self._selectors.commentForm).length) {
                        self.renderPartial('commentForm', parent, {
                            feedbackPrompt: self._i18n['HELPFUL_NEGATIVE_REASON_' + value.toUpperCase() + '_EXTRA']
                        });
                    }

                    self.options.onRedraw && self.options.onRedraw();
                });
            },

            commentForm: function(element) {
                var commentForm = element.find(this._selectors.commentForm);
                var textarea = commentForm.find('textarea');

                // HACK: IE10-11 have a broken placeholder implementation on textareas
                textarea.val('');

                this._limitLength(textarea);
                this._bindHandler('textCounter', commentForm);
            }
        },

        _renderTemplate: function(key, data) {
            data = data || {};

            var templateData = {
                cssClasses: this.options.cssClasses,
                settings: this.options.settings,
                i18n: this._i18n,
                instanceid: this._instanceid
            };

            return AKPAnalytics.util.interpolateText(this.options.templates[key], $.extend({}, templateData, data));
        },

        _create: function() {
            this.element.empty();
            this.renderPartial('main', this.element);

            return $.when(this.options.onCreate && this.options.onCreate.call(this, this.element)).then(function() {
                return this.refresh();
            }.bind(this));
        },

        _destroy: function () {
            this.element.empty();
        },

        _serializeForm: function(form) {
            var hasIssues = !!form.find('[role="radio"]').length;

            if (!hasIssues) {
                return {
                    comment: $.trim(form.find('textarea').val())
                };
            } else {
                var selected = form.find('[role="radio"][aria-checked="true"]');

                if (!selected.length) {
                    return {};
                } else {
                    if (selected.is('[data-allow-feedback="true"]')) {
                        return {
                            issue: selected.attr('data-value'),
                            comment: $.trim(selected.parent().find('textarea').val())
                        };
                    } else {
                        return {
                            issue: selected.attr('data-value')
                        };
                    }
                }
            }
        },

        _validateForm: function(form) {
            var data = this._serializeForm(form);
            var valid = !!Object.keys(data).length;

            for (var field in data) {
                if (data.hasOwnProperty(field)) {
                    valid = valid && !!data[field];
                }
            }

            return valid;
        },

        _bindHandler: function(handler) { // ...args
            this._handlers[handler].apply(this, Array.prototype.slice.call(arguments, 1));
        },

        _handlers: {
            form: function(parentElement) {
                var self = this;

                parentElement.on('submit', this._selectors.feedbackForm, function(e) {
                    var form = $(this);

                    e.preventDefault();
                    if (!self._validateForm(form)) return;

                    var formData = self._serializeForm(form);
                    var data = $.extend(self.loggingHelper.getData(), {
                        e: self.options.feedback_event,
                        uc: formData.comment,
                        hi: formData.issue
                    });

                    self.loggingHelper._logger.log(data);
                    self.__userFeedbackGiven = true;

                    self.refresh();
                });
            },

            radioButtons: function(parentElement, callback) {
                var self = this;

                parentElement.on('click', '[role="radio"]', function(e) {
                    if ((this.getAttribute('aria-disabled') === 'true') || (this.getAttribute('aria-checked') === 'true')) return;

                    $(this).closest('[role="radiogroup"]').find('[role="radio"][aria-checked="true"]').attr('aria-checked', 'false');
                    this.setAttribute('aria-checked', 'true');

                    self._updateSubmitState($(this).closest('form'));

                    callback && callback.call(this, this.getAttribute('data-value'));
                });

                parentElement.on('keydown', '[role="radio"]', function(e) {
                    if (e.keyCode === 13 || e.keyCode === 32) {
                        this.click();
                    }
                });

                parentElement.on('click', '[role="radio"] + label', function(e) {
                    this.previousSibling.click();
                });
            },

            textCounter: function(parentElement) {
                var self = this;

                parentElement.on('keyup', 'textarea', function() {
                    self._updateSubmitState($(this).closest('form'));

                    self._updateCounter(this);
                });
            }
        },

        _updateCounter: function(element) {
            var el = $(element);
            var comment = el.val();
            var maxlength = parseInt(el.attr('maxlength'), 10);
            var counter = el.next(this._selectors.commentCounter);

            counter.html(comment.length + '/' + maxlength);

            if (comment.length === maxlength) {
                counter.addClass('limit-reached');
            } else {
                counter.removeClass('limit-reached');
            }
        },

        _hasCTAforValueState: function(value, state) {
            if (this.options.settings.callToAction[value]) {
                return this.options.settings.callToAction[value].show &&
                       this.options.settings.callToAction[value].after === state;
            } else {
                return false;
            }
        },

        _renderCTASection: function(value) {
            var ctaElement = this.element.find(this._selectors.ctaSectionContainer);

            ctaElement.empty();

            if (value) {
                this.renderPartial(this.options.settings.callToAction[value].templateName, ctaElement);
            }

            ctaElement.attr('aria-hidden', !value);
        },

        refresh: function(currentValue) {
            var self = this;

            currentValue = currentValue || this.loggingHelper.getValue();

            if (currentValue !== undefined) {
                var statusMessage = "";
                var statusArea = this.element.find(this._selectors.statusBar);
                var feedbackForm = this.element.find(this._selectors.feedbackForm);

                feedbackForm.empty();

                if (this.options.settings.commentsEnabled && this.__userStateChanged && !this.__userFeedbackGiven) {
                    statusMessage = (currentValue === 'true') ? this._i18n.HELPFUL_POSITIVE_FEEDBACK : this._i18n.HELPFUL_NEGATIVE_FEEDBACK;

                    if (currentValue === 'true') {
                        this.renderPartial('feedbackYes', feedbackForm, {
                            commentForm: this._renderTemplate('commentForm', {
                                feedbackPrompt: self._i18n.HELPFUL_POSITIVE_TITLE
                            })
                        });
                    } else {
                        this.renderPartial('feedbackNo', feedbackForm);
                    }

                    feedbackForm.attr('aria-hidden', 'false');

                    if (this._hasCTAforValueState(currentValue, 'value')) {
                        this._renderCTASection(currentValue);
                    } else {
                        this._renderCTASection(null);
                    }
                } else if (!this.__userStateChanged || this.__userFeedbackGiven || !this.options.settings.commentsEnabled) {
                    statusMessage = this._i18n.HELPFUL_COMMENT_SENT;

                    this._disableButtons();
                    feedbackForm.attr('aria-hidden', 'true');

                    if (this.__userStateChanged && this.__userFeedbackGiven) {
                        if (this._hasCTAforValueState(currentValue, 'feedback')) {
                            this._renderCTASection(currentValue);
                        }
                    }
                }

                statusArea.html(statusMessage).attr('aria-hidden', 'false');
            }

            this.options.onRedraw && this.options.onRedraw();

            return this;
        }
    });

    $.fn.akp_helpful = function(options) {
        var widget = this.data("autodeskAKPHelpful");

        if (widget) {
            switch (options) {
            case "option":
                return widget.setOptions(arguments[1]);
            case "destroy":
                widget._destroy();
                this.removeData("autodeskAKPHelpful");

                return;
            default:
                return widget;
            }
        } else {
            widget = new AKPHelpful(this, options);

            this.data("autodeskAKPHelpful", widget);
            return widget;
        }
    };

    // register on AKPAnalytics as a plugin
    window.AKPAnalytics = window.AKPAnalytics || {};
    window.AKPAnalytics.plugins = window.AKPAnalytics.plugins || {};
    window.AKPAnalytics.plugins.AKPHelpful = AKPHelpful;

    AKPHelpful._languageCodes = {
        "CHS": "zh-Hans",
        "CHT": "zh-Hant",
        "CSY": "cs",
        "DEU": "de",
        "ENU": "en",
        "ENC": "en-CA",
        "ENG": "en-GB",
        "ESP": "es",
        "FRA": "fr",
        "FRC": "fr-CA",
        "HUN": "hu",
        "ITA": "it",
        "JPN": "ja",
        "KOR": "ko",
        "NLD": "nl",
        "PLK": "pl",
        "PTB": "pt-BR",
        "PTG": "pt-PT",
        "ROU": "ro",
        "RUS": "ru",
        "SWE": "sv",
        "TUR": "tr"
    };

    AKPHelpful._UPIfromISO = (function() {
        var getUPI = function(lang) {
            return Object.keys(AKPHelpful._languageCodes).filter(function(key) {
                if (AKPHelpful._languageCodes[key] && (AKPHelpful._languageCodes[key].toLowerCase() === lang.toLowerCase())) return true;
            }).reduce(function(acc, value) {
                return value;
            }, '');
        };

        return function(languageCode) {
            var hasLocale = (languageCode.indexOf('-') !== -1);

            return getUPI(languageCode) || (hasLocale && getUPI(languageCode.split('-')[0])) || 'ENU';
        };
    })();

    AKPHelpful.i18n = {
        "ENU": {
            HELPFUL_HEADER: "Was this helpful?",
            HELPFUL_YES: "Yes",
            HELPFUL_NO: "No",
            HELPFUL_POSITIVE_FEEDBACK: "Thanks for letting us know!",
            HELPFUL_NEGATIVE_FEEDBACK: "Thanks for letting us know!",
            HELPFUL_POSITIVE_TITLE: "Tell us what you liked about this article.",
            HELPFUL_NEGATIVE_TITLE: "Why wasn't this article helpful?",
            HELPFUL_NEGATIVE_REASON_QUALITY: "The information was incorrect, incomplete, or confusing.",
            HELPFUL_NEGATIVE_REASON_FINDABILITY: "I got to this content, but it didn't answer my question.",
            HELPFUL_NEGATIVE_REASON_LOCALIZATION: "I couldn't understand the translation.",
            HELPFUL_NEGATIVE_REASON_POLICY: "I found the information I was looking for, but I disagree with your policy.",
            HELPFUL_NEGATIVE_REASON_OTHER: "Other",
            HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA: "Please tell us what was wrong with the article.",
            HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA: "Please tell us what you were looking for, and the search terms you used.",
            HELPFUL_NEGATIVE_REASON_OTHER_EXTRA: "Enter comments here.",
            HELPFUL_COMMENT_SUBMIT: "Submit feedback",
            HELPFUL_COMMENT_SENT: "Thank you. We've recorded your feedback.",
            HELPFUL_COMMENT_PLACEHOLDER_TEXT: "NOTE: Your comments here can only be seen by our editor.",
            HELPFUL_CTA_SECTION_TITLE: "Explore other ways to get help",
            HELPFUL_CTA_SECTION_FORUMS_TITLE: "Autodesk Forums",
            HELPFUL_CTA_SECTION_FORUMS_TEXT: "Visit the Autodesk forums to find solutions posted by the community and to ask questions.",
            HELPFUL_CTA_SECTION_FORUMS_BUTTON: "Visit forums",
            HELPFUL_CTA_SECTION_FORUMS_URL: "https://forums.autodesk.com",
            HELPFUL_CTA_SECTION_SUPPORT_TITLE: "Questions?",
            HELPFUL_CTA_SECTION_SUPPORT_TEXT: "Let us help you select a topic and find the right support agent.",
            HELPFUL_CTA_SECTION_SUPPORT_BUTTON: "Contact support",
            HELPFUL_CTA_SECTION_SUPPORT_URL: "https://knowledge.autodesk.com/contact-support"
        },
        "CHS": {
            "HELPFUL_HEADER": "此文章是否有用？",
            "HELPFUL_YES": "是",
            "HELPFUL_NO": "否",
            "HELPFUL_POSITIVE_FEEDBACK": "感谢您提供反馈！",
            "HELPFUL_NEGATIVE_FEEDBACK": "感谢您提供反馈！",
            "HELPFUL_POSITIVE_TITLE": "请告诉我们您喜欢此文章的哪些内容。",
            "HELPFUL_NEGATIVE_TITLE": "为什么认为此文章没有用？",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "所含信息不正确、不完整或导致混淆。",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "查看此内容后，我的问题并未得到解决。",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "我看不懂翻译内容。",
            "HELPFUL_NEGATIVE_REASON_POLICY": "我找到了要找的信息，但对你们的策略有些异议。",
            "HELPFUL_NEGATIVE_REASON_OTHER": "其他。",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "请告诉我们此文章有什么问题。",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "请告诉我们您要查找的内容，以及您使用的搜索词。",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "请在此处输入评论。",
            "HELPFUL_COMMENT_SUBMIT": "提交反馈",
            "HELPFUL_COMMENT_SENT": "非常感谢！我们已经对您的反馈进行了记录。",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "注意：只有我们的编辑人员能够看到您在此处输入的评论。",
            "HELPFUL_CTA_SECTION_TITLE": "探索其他获取帮助的方法",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "欧特克论坛",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "访问欧特克论坛，查找社区发布的解决方案，并咨询问题。",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "访问论坛",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com/t5/zhong-wen/ct-p/531",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "有问题吗？",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "让我们帮您选择主题并查找适当的支持人员。",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "联系技术支持",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/zh-hans/contact-support"
        },
        "CHT": {
            "HELPFUL_HEADER": "此內容是否有用？",
            "HELPFUL_YES": "是",
            "HELPFUL_NO": "否",
            "HELPFUL_POSITIVE_FEEDBACK": "感謝您告知！",
            "HELPFUL_NEGATIVE_FEEDBACK": "感謝您告知！",
            "HELPFUL_POSITIVE_TITLE": "請告訴我們，您認為本文值得讚賞之處。",
            "HELPFUL_NEGATIVE_TITLE": "為什麼本文不實用？",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "資訊不正確、不完整或含糊不清。",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "我從頭到尾瀏覽了內容，但找不到我問題的解答。",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "譯文艱澀難懂。",
            "HELPFUL_NEGATIVE_REASON_POLICY": "我找到了要找的資訊，但我不同意貴公司的政策。",
            "HELPFUL_NEGATIVE_REASON_OTHER": "其他。",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "請告訴我們，文章有哪些不合宜之處。",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "請告訴我們您要尋找的內容，以及您使用的搜尋詞彙。",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "在此處輸入意見。",
            "HELPFUL_COMMENT_SUBMIT": "提交意見反應",
            "HELPFUL_COMMENT_SENT": "謝謝！我們已記錄您的意見反應。",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "注意事項：只有編輯人員才會看見您在此處提供的意見反應。",
            "HELPFUL_CTA_SECTION_TITLE": "探索可獲得協助的其他途徑",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Autodesk 論壇",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "請造訪 Autodesk 論壇，以尋找社群成員張貼的解決方案和提出問題。",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "造訪論壇",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "有問題嗎？",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "讓我們協助您選擇主題並找到正確的支援人員。",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "聯絡支援人員",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/contact-support"
        },
        "CSY": {
            "HELPFUL_HEADER": "Byly tyto informace užitečné?",
            "HELPFUL_YES": "Ano",
            "HELPFUL_NO": "Ne",
            "HELPFUL_POSITIVE_FEEDBACK": "Děkujeme vám za poskytnutí informací.",
            "HELPFUL_NEGATIVE_FEEDBACK": "Děkujeme vám za poskytnutí informací.",
            "HELPFUL_POSITIVE_TITLE": "Řekněte nám, co se vám na tomto článku líbilo.",
            "HELPFUL_NEGATIVE_TITLE": "Proč nebyl tento článek užitečný?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "Informace byly nesprávné, neúplné nebo matoucí.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "Našel/našla jsem tento obsah, ale nedostal/a jsem odpověď na svoji otázku.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "Neporozuměl/a jsem překladu.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "Našel/našla jsem informace, které jsem hledal/a, ale nesouhlasím s vašimi zásadami.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Jiné.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Dejte nám vědět, co bylo s tímto článkem špatně.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Dejte nám vědět, co jste hledali a jaká slova jste použili při vyhledávání.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Zde napište komentář.",
            "HELPFUL_COMMENT_SUBMIT": "Odeslat zpětnou vazbu",
            "HELPFUL_COMMENT_SENT": "Děkujeme vám. Zaregistrovali jsme vaši zpětnou vazbu.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "POZNÁMKA: Váš komentář uvedený zde uvidí pouze náš editor.",
            "HELPFUL_CTA_SECTION_TITLE": "Prozkoumejte další způsoby získání pomoci",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Fóra společnosti Autodesk",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Navštivte diskusní fóra společnosti Autodesk, na kterých můžete hledat řešení od členů komunity a vznášet dotazy.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Navštívit diskusní fóra",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Máte dotazy?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Dovolte, abychom vám pomohli s výběrem tématu a nalezením správného zástupce podpory.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Kontaktovat podporu",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/contact-support"
        },
        "DEU": {
            "HELPFUL_HEADER": "War dies hilfreich?",
            "HELPFUL_YES": "Ja",
            "HELPFUL_NO": "Nein",
            "HELPFUL_POSITIVE_FEEDBACK": "Vielen Dank für Ihr Feedback!",
            "HELPFUL_NEGATIVE_FEEDBACK": "Vielen Dank für Ihr Feedback!",
            "HELPFUL_POSITIVE_TITLE": "Teilen Sie uns mit, was Ihnen an diesem Artikel gefallen hat.",
            "HELPFUL_NEGATIVE_TITLE": "Warum war dieser Artikel nicht hilfreich?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "Die Informationen waren nicht korrekt, unvollständig oder irreführend.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "Der Artikel hat meine Frage nicht beantwortet.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "Ich habe die Übersetzung nicht verstanden.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "Ich habe die gesuchten Informationen gefunden, aber ich stimme der Richtlinie nicht zu.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Andere Gründe",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Teilen Sie uns mit, was Ihnen an diesem Artikel nicht gefallen hat.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Teilen Sie uns mit, wonach Sie gesucht und welche Suchbegriffe Sie verwendet haben.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Geben Sie hier Ihre Kommentare ein.",
            "HELPFUL_COMMENT_SUBMIT": "Feedback senden",
            "HELPFUL_COMMENT_SENT": "Vielen Dank! Wir haben Ihr Feedback erhalten.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "ANMERKUNG: Ihre Kommentare sind nur für unseren Editor sichtbar.",
            "HELPFUL_CTA_SECTION_TITLE": "Entdecken Sie andere Hilfeoptionen",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Autodesk-Foren",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Besuchen Sie die Autodesk-Foren, um von der Community bereitgestellte Lösungen einzusehen und Fragen zu stellen.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Foren besuchen",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com/t5/deutsch/ct-p/59",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Fragen?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Wir helfen Ihnen gerne dabei, ein Thema auszuwählen und den richtigen Supportmitarbeiter zu finden.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Support kontaktieren",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/de/contact-support"
        },
        "ENC": {
            "HELPFUL_HEADER": "Was this helpful?",
            "HELPFUL_YES": "Yes",
            "HELPFUL_NO": "No",
            "HELPFUL_POSITIVE_FEEDBACK": "Thanks for letting us know!",
            "HELPFUL_NEGATIVE_FEEDBACK": "Thanks for letting us know!",
            "HELPFUL_POSITIVE_TITLE": "Tell us what you liked about this article.",
            "HELPFUL_NEGATIVE_TITLE": "Why wasn't this article helpful?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "The information was incorrect, incomplete, or confusing.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "I got to this content, but it didn't answer my question.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "I couldn't understand the translation.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "I found the information I was looking for, but I disagree with your policy.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Other.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Please tell us what was wrong with the article.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Please tell us what you were looking for, and the search terms you used.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Enter comments here.",
            "HELPFUL_COMMENT_SUBMIT": "Submit feedback",
            "HELPFUL_COMMENT_SENT": "Thank you. We've recorded your feedback.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "NOTE: Your comments here can only be seen by our editor.",
            "HELPFUL_CTA_SECTION_TITLE": "Explore other ways to get help",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Autodesk Forums",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Visit the Autodesk Forums to find solutions posted by the community and to ask questions.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Visit forums",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Questions?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Let us help you select a topic and find the right support agent.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Contact Support",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/contact-support"
        },
        "ENG": {
            "HELPFUL_HEADER": "Was this helpful?",
            "HELPFUL_YES": "Yes",
            "HELPFUL_NO": "No",
            "HELPFUL_POSITIVE_FEEDBACK": "Thanks for letting us know!",
            "HELPFUL_NEGATIVE_FEEDBACK": "Thanks for letting us know!",
            "HELPFUL_POSITIVE_TITLE": "Tell us what you liked about this article.",
            "HELPFUL_NEGATIVE_TITLE": "Why wasn't this article helpful?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "The information was incorrect, incomplete, or confusing.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "I got to this content, but it didn't answer my question.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "I couldn't understand the translation.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "I found the information I was looking for, but I disagree with your policy.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Other.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Please tell us what was wrong with the article.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Please tell us what you were looking for, and the search terms you used.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Enter comments here.",
            "HELPFUL_COMMENT_SUBMIT": "Submit feedback",
            "HELPFUL_COMMENT_SENT": "Thank you. We've recorded your feedback.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "NOTE: Your comments here can only be seen by our editor.",
            "HELPFUL_CTA_SECTION_TITLE": "Explore other ways to get help",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Autodesk Forums",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Visit the Autodesk forums to find solutions posted by the community and to ask questions.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Visit forums",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Questions?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Let us help you select a topic and find the right support agent.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Contact support",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/contact-support"
        },
        "ESP": {
            "HELPFUL_HEADER": "¿Le ha resultado útil?",
            "HELPFUL_YES": "Sí",
            "HELPFUL_NO": "No",
            "HELPFUL_POSITIVE_FEEDBACK": "Gracias por compartir su opinión con nosotros.",
            "HELPFUL_NEGATIVE_FEEDBACK": "Gracias por compartir su opinión con nosotros.",
            "HELPFUL_POSITIVE_TITLE": "Díganos lo que le ha gustado de este artículo.",
            "HELPFUL_NEGATIVE_TITLE": "¿Por qué no le ha resultado útil este artículo?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "La información es incorrecta, confusa o está incompleta.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "Se me propuso este contenido, pero no respondió a mi pregunta.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "No he entendido la traducción.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "He encontrado la información que buscaba, pero no estoy de acuerdo con su política.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Otro.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Indíquenos cuál ha sido el problema con el artículo.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Indíquenos qué estaba buscando y los términos de búsqueda que empleó.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Escriba los comentarios aquí.",
            "HELPFUL_COMMENT_SUBMIT": "Enviar comentarios",
            "HELPFUL_COMMENT_SENT": "Gracias. Hemos registrado sus comentarios.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "NOTA: Solo nuestro editor puede ver los comentarios que introduzca aquí.",
            "HELPFUL_CTA_SECTION_TITLE": "Encuentre otras formas para obtener ayuda",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Foros de Autodesk",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Visite los foros de Autodesk para buscar las soluciones que ha publicado la comunidad y realizar preguntas.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Visitar los foros",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com/t5/espanol/ct-p/157",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "¿Alguna pregunta?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Le ayudaremos a seleccionar un tema y a buscar el agente de soporte correcto.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Ponerse en contacto con el soporte técnico",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/es/contact-support"
        },
        "FRA": {
            "HELPFUL_HEADER": "Cet article vous a-t-il été utile ?",
            "HELPFUL_YES": "Oui",
            "HELPFUL_NO": "Non",
            "HELPFUL_POSITIVE_FEEDBACK": "Merci de nous avoir donné votre avis !",
            "HELPFUL_NEGATIVE_FEEDBACK": "Merci de nous avoir donné votre avis !",
            "HELPFUL_POSITIVE_TITLE": "Qu'avez-vous apprécié dans cet article ?",
            "HELPFUL_NEGATIVE_TITLE": "Pourquoi cet article ne vous a-t-il pas été utile ?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "Les informations étaient incorrectes, incomplètes ou peu claires.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "Le contenu de cet article ne m'a pas permis de trouver de réponse à ma question.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "Je n'ai pas compris la traduction.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "J'ai trouvé ce que je cherchais, mais je ne suis pas d'accord avec votre politique.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Autre.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Merci de nous indiquer les points pouvant être améliorés dans cet article.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Merci de nous indiquer ce que vous recherchiez, ainsi que les termes utilisés lors de votre recherche.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Entrez vos commentaires ici.",
            "HELPFUL_COMMENT_SUBMIT": "Envoyer les commentaires",
            "HELPFUL_COMMENT_SENT": "Merci. Vos commentaires ont été enregistrés.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "Remarque : seul notre éditeur peut voir ces commentaires.",
            "HELPFUL_CTA_SECTION_TITLE": "Autres moyens d'obtenir de l'aide",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Forums Autodesk",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Visitez les forums Autodesk pour consulter les solutions publiées par la communauté et pour poser des questions.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Visiter les forums",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com/t5/francais/ct-p/191",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Des questions ?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Nous pouvons vous aider à choisir une rubrique et à contacter un agent de l'assistance.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Contacter l'assistance",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/fr/contact-support"
        },
        "FRC": {
            "HELPFUL_HEADER": "Cet article vous a-il été utile?",
            "HELPFUL_YES": "Oui",
            "HELPFUL_NO": "Non",
            "HELPFUL_POSITIVE_FEEDBACK": "Merci de nous avoir donné votre avis!",
            "HELPFUL_NEGATIVE_FEEDBACK": "Merci de nous avoir donné votre avis!",
            "HELPFUL_POSITIVE_TITLE": "Qu'avez-vous apprécié dans cet article?",
            "HELPFUL_NEGATIVE_TITLE": "Pourquoi cet article ne vous a-il pas été utile?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "Les informations étaient incorrectes, incomplètes ou peu claires.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "Le contenu de cet article ne m'a pas permis de trouver de réponse à ma question.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "Je n'ai pas compris la traduction.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "J'ai trouvé ce que je cherchais, mais je ne suis pas d'accord avec votre politique.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Autre.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Merci de nous indiquer les points pouvant être améliorés dans cet article.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Merci de nous indiquer ce que vous recherchiez, ainsi que les termes utilisés lors de votre recherche.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Entrez vos commentaires ici.",
            "HELPFUL_COMMENT_SUBMIT": "Envoyer les commentaires",
            "HELPFUL_COMMENT_SENT": "Merci. Vos commentaires ont été enregistrés.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "Remarque : seul notre éditeur peut voir ces commentaires.",
            "HELPFUL_CTA_SECTION_TITLE": "Autres moyens d'obtenir de l'aide",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Forums Autodesk",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Visitez les forums Autodesk pour consulter les solutions publiées par la communauté et pour poser des questions.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Visiter les forums",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com/t5/francais/ct-p/191",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Des questions?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Laissez-nous vous aider à sélectionner une rubrique et à rechercher l'agent(e) de soutien correspondant(e).",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Contacter l'assistance",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/fr/contact-support"
        },
        "HUN": {
            "HELPFUL_HEADER": "Hasznosnak találta?",
            "HELPFUL_YES": "Igen",
            "HELPFUL_NO": "Nem",
            "HELPFUL_POSITIVE_FEEDBACK": "Köszönjük, hogy elmondta véleményét!",
            "HELPFUL_NEGATIVE_FEEDBACK": "Köszönjük, hogy elmondta véleményét!",
            "HELPFUL_POSITIVE_TITLE": "Ossza meg velünk, mi tetszett Önnek ebben a cikkben.",
            "HELPFUL_NEGATIVE_TITLE": "Miért nem volt hasznos ez a cikk?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "Az információ helytelen, hiányos vagy zavaros volt.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "Ehhez a tartalomhoz jutottam, de nem kaptam választ a kérdésemre.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "Nem értettem a fordítást.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "Megtaláltam a keresett információt, de nem értek egyet az irányelvekkel.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Egyéb.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Kérjük, mondja el, mi a baj a cikkel.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Kérjük, mondja el, mit keresett, és milyen keresőkifejezést használt.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Itt írhatja le megjegyzéseit.",
            "HELPFUL_COMMENT_SUBMIT": "Visszajelzés elküldése",
            "HELPFUL_COMMENT_SENT": "Köszönjük! Rögzítettük visszajelzését.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "MEGJEGYZÉS: Az itt leírt észrevételeit csak a szerkesztőnk láthatja.",
            "HELPFUL_CTA_SECTION_TITLE": "Ismerje meg, milyen más módon kaphat még segítséget",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Autodesk fórumok",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Látogassa meg az Autodesk fórumait, ahol megtalálja a közösség által közzétett megoldásokat, és kérdéseit is felteheti.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Fórumok megtekintése",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Kérdései vannak?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Segítünk a téma kiválasztásában és a megfelelő támogatási szakember megkeresésében.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Kapcsolatfelvétel a támogatással",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/contact-support"
        },
        "ITA": {
            "HELPFUL_HEADER": "Queste informazioni sono state utili?",
            "HELPFUL_YES": "Sì",
            "HELPFUL_NO": "No",
            "HELPFUL_POSITIVE_FEEDBACK": "Grazie per la risposta.",
            "HELPFUL_NEGATIVE_FEEDBACK": "Grazie per la risposta.",
            "HELPFUL_POSITIVE_TITLE": "Aggiungi un commento indicando cosa ti è piaciuto dell'articolo.",
            "HELPFUL_NEGATIVE_TITLE": "Perché l'articolo non è stato utile?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "Le informazioni non sono corrette, sono incomplete o generano confusione.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "Ho visualizzato questo contenuto, ma non risponde alla mia domanda.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "Non ho capito la traduzione.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "Ho trovato le informazioni che cercavo, ma non sono d'accordo con i vostri criteri.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Altro.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Indicaci quale problema hai riscontrato nell'articolo.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Indicaci cosa stavi cercando e quali termini di ricerca hai utilizzato.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Inserisci qui i tuoi commenti.",
            "HELPFUL_COMMENT_SUBMIT": "Invia feedback",
            "HELPFUL_COMMENT_SENT": "Grazie. Abbiamo registrato il tuo feedback.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "NOTA: i commenti inseriti qui possono essere visti solo dal nostro editor.",
            "HELPFUL_CTA_SECTION_TITLE": "Esplora altri modi per ottenere assistenza",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Forum di Autodesk",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Visita i forum di Autodesk per trovare le soluzioni pubblicate dalla community e porre domande.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Visita i forum",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Domande?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Consentici di aiutarti a selezionare un argomento e a trovare l'agente del supporto appropriato.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Contatta il supporto tecnico",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/it/contact-support"
        },
        "JPN": {
            "HELPFUL_HEADER": "この情報は役に立ちましたか?",
            "HELPFUL_YES": "はい",
            "HELPFUL_NO": "いいえ",
            "HELPFUL_POSITIVE_FEEDBACK": "ご意見ありがとうございます。",
            "HELPFUL_NEGATIVE_FEEDBACK": "ご意見ありがとうございます。",
            "HELPFUL_POSITIVE_TITLE": "この記事で気に入った点を教えてください。",
            "HELPFUL_NEGATIVE_TITLE": "この記事が役に立たなかった理由を教えてください。",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "情報が誤っていたり内容が不十分で、わかりにくかったから",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "この情報にたどり着いたものの、疑問は解決しなかったから",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "翻訳が理解できなかったから",
            "HELPFUL_NEGATIVE_REASON_POLICY": "必要な情報は見つかったが、方針に賛同できないから",
            "HELPFUL_NEGATIVE_REASON_OTHER": "その他",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "この記事で誤っている点を教えてください。",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "お探しの情報と検索に使用した語句を教えてください。",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "こちらにコメントを入力してください。",
            "HELPFUL_COMMENT_SUBMIT": "フィードバックを送信",
            "HELPFUL_COMMENT_SENT": "ご協力ありがとうございました。フィードバックは記録されました。",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "注: いただいたコメントはオートデスクの編集担当者以外には閲覧されません。",
            "HELPFUL_CTA_SECTION_TITLE": "その他のヘルプを試す",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "オートデスク ディスカッション フォーラム",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "オートデスク ディスカッション フォーラムにアクセスして、コミュニティに投稿された解決策を検索したり、質問したりすることができます。",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "フォーラムにアクセスする",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com/t5/ri-ben-yu/ct-p/511",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "何か質問はありますか?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "こちらからトピックをお選びください。適切なサポート スペシャリストにおつなぎします。",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "サポートに問い合わせる",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/ja/contact-support"
        },
        "KOR": {
            "HELPFUL_HEADER": "이 자료가 도움이 되었습니까?",
            "HELPFUL_YES": "예",
            "HELPFUL_NO": "아니요",
            "HELPFUL_POSITIVE_FEEDBACK": "알려주셔서 감사합니다!",
            "HELPFUL_NEGATIVE_FEEDBACK": "알려주셔서 감사합니다!",
            "HELPFUL_POSITIVE_TITLE": "이 문서에서 마음에 드는 부분을 알려주십시오.",
            "HELPFUL_NEGATIVE_TITLE": "이 문서가 도움이 되지 않은 이유는 무엇입니까?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "정보가 잘못되었거나 불완전하거나 혼동을 유발합니다.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "이 내용을 읽어도 궁금한 점이 해결되지 않았습니다.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "번역을 이해할 수 없었습니다.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "찾고 있던 정보를 찾았지만 귀사의 정책에 동의하지 않습니다.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "기타.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "문서에 어떤 문제가 있었는지 알려주시기 바랍니다.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "찾고 있던 내용은 무엇이며 사용한 검색어는 무엇이었는지 알려주시기 바랍니다.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "여기에 의견을 입력하십시오.",
            "HELPFUL_COMMENT_SUBMIT": "피드백 제출",
            "HELPFUL_COMMENT_SENT": "감사합니다. 여러분의 피드백을 기록했습니다.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "참고: 여기에 입력하신 의견은 당사의 편집기로만 보실 수 있습니다.",
            "HELPFUL_CTA_SECTION_TITLE": "도움을 구할 수 있는 다른 방법 알아보기",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Autodesk 포럼",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Autodesk 포럼을 방문하여 커뮤니티에 게시된 해결 방법을 찾고 질문을 하십시오.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "포럼 방문",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "질문이 있으십니까?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "관련 주제를 선택하고 적합한 지원 담당자를 찾을 수 있도록 도와드리겠습니다.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "지원 부서에 문의",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/ko/contact-support"
        },
        "NLD": {
            "HELPFUL_HEADER": "Was dit nuttig?",
            "HELPFUL_YES": "Ja",
            "HELPFUL_NO": "Nee",
            "HELPFUL_POSITIVE_FEEDBACK": "Bedankt voor uw feedback!",
            "HELPFUL_NEGATIVE_FEEDBACK": "Bedankt voor uw feedback!",
            "HELPFUL_POSITIVE_TITLE": "Laat ons weten wat u handig vond aan dit artikel.",
            "HELPFUL_NEGATIVE_TITLE": "Waarom was dit artikel niet nuttig?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "De informatie was onjuist, onvolledig of verwarrend.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "Ik ben bij deze inhoud beland, maar mijn vraag is niet beantwoord.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "Ik kon de vertaling niet begrijpen.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "Ik heb de informatie gevonden die ik zocht, maar ik ben het oneens met jullie beleid.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Anders.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Laat ons weten wat er mis is met dit artikel.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Laat ons weten waar u naar zocht en met welke zoektermen.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Voer hier uw opmerkingen in.",
            "HELPFUL_COMMENT_SUBMIT": "Feedback verzenden",
            "HELPFUL_COMMENT_SENT": "Bedankt. We hebben uw feedback vastgelegd.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "LET OP: uw opmerkingen hier zijn alleen zichtbaar voor onze redacteur.",
            "HELPFUL_CTA_SECTION_TITLE": "Ontdek andere manieren van ondersteuning",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Autodesk-forums",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Ga naar de Autodesk-forums om oplossingen te vinden die zijn geplaatst door de community en om vragen te stellen.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Naar de forums",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Contact opnemen met ondersteuning",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Hebt u nog vragen? Laat ons u helpen een onderwerp te selecteren en u te verwijzen naar de juiste ondersteuningsmedewerker.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Contact opnemen met ondersteuning",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/contact-support"
        },
        "PLK": {
            "HELPFUL_HEADER": "Czy ten artykuł był pomocny?",
            "HELPFUL_YES": "Tak",
            "HELPFUL_NO": "Nie",
            "HELPFUL_POSITIVE_FEEDBACK": "Dziękujemy za informację!",
            "HELPFUL_NEGATIVE_FEEDBACK": "Dziękujemy za informację!",
            "HELPFUL_POSITIVE_TITLE": "Co podobało Ci się w tym artykule?",
            "HELPFUL_NEGATIVE_TITLE": "Dlaczego artykuł nie był pomocny?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "Informacje były błędne, niepełne lub niejasne.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "Podane informacje nie odpowiadają na moje pytanie.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "Tłumaczenie było niezrozumiałe.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "Udało mi się znaleźć potrzebne informacje, ale nie zgadzam się z Waszymi zasadami.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Inny powód.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Prosimy o informację, na czym konkretnie polega problem z tym artykułem.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Prosimy o podanie, czego dotyczyło wyszukiwanie, wraz z terminami użytymi przy wyszukiwaniu.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Tutaj wpisz swoje uwagi.",
            "HELPFUL_COMMENT_SUBMIT": "Wyślij opinię",
            "HELPFUL_COMMENT_SENT": "Dziękujemy. Opinia została zapisana.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "UWAGA: Wpisane tutaj uwagi są widoczne tylko dla naszego redaktora.",
            "HELPFUL_CTA_SECTION_TITLE": "Poznaj inne sposoby uzyskania pomocy",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Forum Autodesk",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Odwiedź forum Autodesk, aby poznawać rozwiązania publikowane przez społeczność użytkowników i zadawać pytania.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Odwiedź forum",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Pytania?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Pomożemy Ci wybrać temat i znaleźć właściwego konsultanta pomocy technicznej.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Skontaktuj się z działem pomocy technicznej",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/contact-support"
        },
        "PTB": {
            "HELPFUL_HEADER": "Isso foi útil?",
            "HELPFUL_YES": "Sim",
            "HELPFUL_NO": "Não",
            "HELPFUL_POSITIVE_FEEDBACK": "Obrigado por nos avisar!",
            "HELPFUL_NEGATIVE_FEEDBACK": "Obrigado por nos avisar!",
            "HELPFUL_POSITIVE_TITLE": "Diga-nos do que você gostou neste artigo.",
            "HELPFUL_NEGATIVE_TITLE": "Por que este artigo não foi útil?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "As informações estavam incorretas, incompletas ou confusas.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "Consultei este conteúdo, mas ele não respondeu à minha pergunta.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "Não consegui entender a tradução.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "Encontrei as informações que eu estava procurando, mas discordo de sua política.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Outros.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Diga-nos o que havia de errado no artigo.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Diga-nos o que você estava procurando e os termos de pesquisa que usou.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Insira comentários aqui.",
            "HELPFUL_COMMENT_SUBMIT": "Enviar comentários",
            "HELPFUL_COMMENT_SENT": "Obrigado. Registramos seus comentários.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "OBSERVAÇÃO: Os comentários aqui só podem ser vistos por nosso editor.",
            "HELPFUL_CTA_SECTION_TITLE": "Explore outras maneiras de obter ajuda",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Fóruns da Autodesk",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Acesse os fóruns da Autodesk para encontrar soluções postadas pela comunidade e fazer perguntas.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Acessar os fóruns",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com/t5/portugues/ct-p/156",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Perguntas?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Se você quiser, podemos ajudá-lo a selecionar um tópico e localizar o agente de suporte correto.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Entrar em contato com o suporte",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/pt-br/contact-support"
        },
        "PTG": {
            "HELPFUL_HEADER": "Este artigo foi útil?",
            "HELPFUL_YES": "Sim",
            "HELPFUL_NO": "Não",
            "HELPFUL_POSITIVE_FEEDBACK": "Obrigado por responder!",
            "HELPFUL_NEGATIVE_FEEDBACK": "Obrigado por responder!",
            "HELPFUL_POSITIVE_TITLE": "Indique-nos porque gostou deste artigo.",
            "HELPFUL_NEGATIVE_TITLE": "Por que razão este artigo não foi útil?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "As informações estavam incorretas, incompletas ou eram confusas.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "Tive acesso a este conteúdo, mas não respondeu à minha pergunta.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "Não consegui compreender a tradução.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "Encontrei as informações que procurava, mas não concordo com a vossa política.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Outros.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Indique-nos o que encontrou de errado neste artigo.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Indique-nos o que procurava e os termos de pesquisa que utilizou.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Introduza os comentários aqui.",
            "HELPFUL_COMMENT_SUBMIT": "Submeter comentários",
            "HELPFUL_COMMENT_SENT": "Obrigado. Os seus comentários foram guardados.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "NOTA: Os comentários aqui introduzidos só podem ser vistos pelo nosso editor.",
            "HELPFUL_CTA_SECTION_TITLE": "Explorar outras formas de obter ajuda",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Fóruns da Autodesk",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Visite os fóruns da Autodesk para encontrar soluções publicadas pela comunidade e fazer perguntas.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Visitar os fóruns ",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Dúvidas?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Se quiser, podemos ajudá-lo a selecionar um tópico e a encontrar o agente de suporte certo.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Contactar o suporte",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/contact-support"
        },
        "ROU": {
            "HELPFUL_HEADER": "A fost util?",
            "HELPFUL_YES": "Da",
            "HELPFUL_NO": "Nu",
            "HELPFUL_POSITIVE_FEEDBACK": "Vă mulțumim pentru că ne-ați informat!",
            "HELPFUL_NEGATIVE_FEEDBACK": "Vă mulțumim pentru că ne-ați informat!",
            "HELPFUL_POSITIVE_TITLE": "Spuneți-ne ce v-a plăcut la acest articol.",
            "HELPFUL_NEGATIVE_TITLE": "De ce nu a fost util acest articol?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "Informațiile au fost incorecte, incomplete sau confuze.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "Am înțeles acest conținut, dar nu a răspuns întrebării mele.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "Nu am putut înțelege traducerea.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "Am găsit informația pe care o căutam, dar nu sunt de acord cu politica dvs.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Altceva.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Spuneți-ne, vă rugăm, ce a fost greșit la articol.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Spuneți-ne, vă rugăm, ce ați căutat și termenii de căutare pe care i-ați folosit.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Introduceți comentariile aici.",
            "HELPFUL_COMMENT_SUBMIT": "Trimiteți feedbackul",
            "HELPFUL_COMMENT_SENT": "Vă mulțumim. V-am înregistrat feedbackul.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "NOTĂ: Comentariile dvs. de aici pot fi văzute doar de editorul nostru.",
            "HELPFUL_CTA_SECTION_TITLE": "Explorați alte modalități de a obține ajutor",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Forumurile Autodesk",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Vizitați forumurile Autodesk pentru a găsi soluții publicate de comunitate și pentru a pune întrebări.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Vizitați forumurile",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Întrebări?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Lăsați-ne să vă ajutăm să selectați un subiect și să găsiți agentul de asistență potrivit.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Contactați asistența",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/contact-support"
        },
        "RUS": {
            "HELPFUL_HEADER": "Была ли эта статья полезной?",
            "HELPFUL_YES": "Да",
            "HELPFUL_NO": "Нет",
            "HELPFUL_POSITIVE_FEEDBACK": "Благодарим за отзыв.",
            "HELPFUL_NEGATIVE_FEEDBACK": "Благодарим за отзыв.",
            "HELPFUL_POSITIVE_TITLE": "Расскажите, что вам понравилось в этой статье.",
            "HELPFUL_NEGATIVE_TITLE": "Почему статья не была полезной для вас?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "Информация была неверной, неполной или непонятной.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "Содержимое статьи не дало ответ на мой вопрос.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "Не удалось понять перевод.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "Мне удалось найти нужную информацию, но политика компании вызывает у меня несогласие.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Другое.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Сообщите, что не так со статьей.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Сообщите, какую информацию вы искали и какие слова использовали в поисковом запросе.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Введите комментарии здесь.",
            "HELPFUL_COMMENT_SUBMIT": "Отправить отзыв",
            "HELPFUL_COMMENT_SENT": "Спасибо. Ваш отзыв учтен.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "ПРИМЕЧАНИЕ. Ваши комментарии может просматривать только наш редактор.",
            "HELPFUL_CTA_SECTION_TITLE": "Узнайте о других способах получения помощи",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Форумы Autodesk",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Посетите форумы Autodesk, чтобы найти предложенные участниками сообщества решения и задать вопросы.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Перейти на форумы",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com/t5/russkiy/ct-p/165",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "У вас есть вопросы?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Мы поможем вам выбрать раздел и найти нужного специалиста технической поддержки.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Обратиться в службу поддержки",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/ru/contact-support"
        },
        "SWE": {
            "HELPFUL_HEADER": "Hjälpte detta dig?",
            "HELPFUL_YES": "Ja",
            "HELPFUL_NO": "Nej",
            "HELPFUL_POSITIVE_FEEDBACK": "Tack för att du berättar för oss!",
            "HELPFUL_NEGATIVE_FEEDBACK": "Tack för att du berättar för oss!",
            "HELPFUL_POSITIVE_TITLE": "Berätta för oss vad du tyckte om den här artikeln.",
            "HELPFUL_NEGATIVE_TITLE": "Varför tyckte du inte att artikeln var användbar?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "Informationen var felaktig, ofullständig eller otydlig.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "Jag kom till det här innehållet, men det gav inte svar på min fråga.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "Jag förstod inte översättningen.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "Jag hittade informationen jag letade efter, men jag håller inte med om er policy.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Annat:",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Berätta för oss vad som var fel med den här artikeln.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Berätta för oss vad du letade efter och vilka sökord du använde.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Skriv dina kommentarer här.",
            "HELPFUL_COMMENT_SUBMIT": "Skicka feedback",
            "HELPFUL_COMMENT_SENT": "Tack! Vi har tagit emot din feedback.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "OBS! Kommentarerna du skriver här kan endast ses av vår redaktör.",
            "HELPFUL_CTA_SECTION_TITLE": "Upptäck andra sätt att få hjälp",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Autodesks forum",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Besök Autodesks forum och hitta lösningar som publicerats av communityn och ställ frågor.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Besök våra forum",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Frågor?",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Låt oss hjälpa dig att välja ämne och få rätt supportperson.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Kontakta supporten",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/contact-support"
        },
        "TUR": {
            "HELPFUL_HEADER": "Bu size yardımcı oldu mu?",
            "HELPFUL_YES": "Evet",
            "HELPFUL_NO": "Hayır",
            "HELPFUL_POSITIVE_FEEDBACK": "Bizimle paylaştığınız için teşekkür ederiz!",
            "HELPFUL_NEGATIVE_FEEDBACK": "Bizimle paylaştığınız için teşekkür ederiz!",
            "HELPFUL_POSITIVE_TITLE": "Bu makalenin sevdiğiniz yönlerini bizimle paylaşın.",
            "HELPFUL_NEGATIVE_TITLE": "Bu makale yardımcı olmadı mı?",
            "HELPFUL_NEGATIVE_REASON_QUALITY": "Bilgiler yanlış, eksik veya muğlaktı.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY": "İçeriği anladım ancak soruma yanıt alamadım.",
            "HELPFUL_NEGATIVE_REASON_LOCALIZATION": "Çeviriyi anlayamadım.",
            "HELPFUL_NEGATIVE_REASON_POLICY": "Aradığım bilgileri buldum ancak politikanıza katılmıyorum.",
            "HELPFUL_NEGATIVE_REASON_OTHER": "Diğer.",
            "HELPFUL_NEGATIVE_REASON_QUALITY_EXTRA": "Lütfen makalede nelerin yanlış olduğunu bizimle paylaşın.",
            "HELPFUL_NEGATIVE_REASON_FINDABILITY_EXTRA": "Lütfen ne aradığınızı ve kullandığınız arama terimlerini bizimle paylaşın.",
            "HELPFUL_NEGATIVE_REASON_OTHER_EXTRA": "Yorumlarınızı buraya girin.",
            "HELPFUL_COMMENT_SUBMIT": "Geri bildirim gönderin",
            "HELPFUL_COMMENT_SENT": "Teşekkür ederiz. Geri bildirimlerinizi kayıt altına aldık.",
            "HELPFUL_COMMENT_PLACEHOLDER_TEXT": "NOT: Buradaki yorumlarınız yalnızca editörümüz tarafından görülebilir.",
            "HELPFUL_CTA_SECTION_TITLE": "Yardım alabileceğiniz diğer yolları keşfedin",
            "HELPFUL_CTA_SECTION_FORUMS_TITLE": "Autodesk Forumları",
            "HELPFUL_CTA_SECTION_FORUMS_TEXT": "Topluluk tarafından gönderilerin çözümlere ulaşmak ve soru sormak için Autodesk forumlarını ziyaret edin.",
            "HELPFUL_CTA_SECTION_FORUMS_BUTTON": "Forumları ziyaret edin",
            "HELPFUL_CTA_SECTION_FORUMS_URL": "https://forums.autodesk.com",
            "HELPFUL_CTA_SECTION_SUPPORT_TITLE": "Destek ile iletişime geçin",
            "HELPFUL_CTA_SECTION_SUPPORT_TEXT": "Yine de sorularınız mı var? Bir konu seçmenize yardım edelim ve sizi doğru destek temsilcisine yönlendirelim.",
            "HELPFUL_CTA_SECTION_SUPPORT_BUTTON": "Destek ile iletişime geçin",
            "HELPFUL_CTA_SECTION_SUPPORT_URL": "https://knowledge.autodesk.com/contact-support"
        }
    };

})(jQuery, window, document);
