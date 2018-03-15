var sixApart_atpComments = {
    /* Vars used throughout comment prep */
    params: {
        loaded: false,
        isAdvanced: false,
        buttonInterval: undefined,
        commenterName: undefined
    },

    prepare: function ( form ) {
        /* Load all ATP Comment depedencies */
        this.loadDependencies();

        /* If we're not dealing with an advanced template set, setup input styling */
        /* Input styling is the logic to show default text in gray but input text in black */
        if(!this.params.isAdvanced) {
            if(document.addEventListener) {
                if( !this.params.isAdvanced && !registrationRequired ){
                    form.author.addEventListener('focus', this.commentInputStyling,false);
                    form.author.addEventListener('blur', this.commentInputStyling, false);
                    if( form.email ){
                        form.email.addEventListener('focus', this.commentInputStyling,false);
                        form.email.addEventListener('blur', this.commentInputStyling, false);                    
                    }
                    if( form.url ){
                        form.url.addEventListener('focus', this.commentInputStyling,false);
                        form.url.addEventListener('blur', this.commentInputStyling, false);                    
                    }
                }
                form.text.addEventListener('keyup', this.commentInputStyling,false);

            } else if(document.attachEvent) {
                if( !this.params.isAdvanced && !registrationRequired ){
                    form.author.attachEvent('onfocus', this.commentInputStyling);
                    form.author.attachEvent('onblur', this.commentInputStyling);
                    if( form.email ){
                        form.email.attachEvent('onfocus', this.commentInputStyling);
                        form.email.attachEvent('onblur', this.commentInputStyling);                    
                    }
                    if( form.url ){
                        form.url.attachEvent('onfocus', this.commentInputStyling);
                        form.url.attachEvent('onblur', this.commentInputStyling);                    
                    }
                }
                form.text.attachEvent('onkeyup', this.commentInputStyling);
            }
        }

        /* If the preview div exists and isn't hidden then add the comment styles */
        if( commentPreview = document.getElementById('comment-preview-confirmation') ) {
            if( this.getStyle(commentPreview,"display") != 'none' ) {
                this.addCommentStyles();
            }
        }
    },

    /* Add our atp-comments stylesheet */
    addCommentStyles: function () {
        var comment_style = document.createElement('link');

        comment_style.setAttribute('rel','stylesheet');
        comment_style.href = '/.shared/css/atp-comments.css';
        comment_style.type = 'text/css';
        comment_style.media = 'screen';

        document.getElementsByTagName('head')[0].appendChild(comment_style);    
    },

    /* Return the value for a style property, given a DOM element */
    getStyle: function ( el, property ) {
        var styleValue;

        if( el.currentStyle ) {
            styleValue = el.currentStyle[ property ];
        } else if( window.getComputedStyle ) {
            styleValue = window.getComputedStyle( el,null ).getPropertyValue( property );
        }

        return styleValue;
    },

    /* Load the required JavaScript dependencies for atpComments */
    loadDependencies: function () {
        var loader;
        var commentPreview;
        var _this = this;

        if (!this.params.loaded) {
            loader = new YAHOO.util.YUILoader();
            loader.addModule({
                name: "atpcomments",
                type: "js",
                fullpath: atpCommentsPath
            });
            loader.addModule({
                name: "jsonrpc",
                type: "js",
                fullpath: "/.shared/js/jsonrpc-client.js"       
            });
            loader.require("yahoo-dom-event", "connection", "json", "jsonrpc", "atpcomments");      
            loader.insert({
                onSuccess: function() {
                    var commentForm = _this.findCommentForm(); /* Set reference to the comment form */
                    var injectDiv;                                          /* Used in adv templates for injecting comment code */
                    var xid;                                                /* Entry XID */

                    if(_this.params.isAdvanced) {
                        xid = _this.getEntryXid(); 

                        /* Inject the comment code before the commentForm */
                        injectDiv = document.createElement('div');
                        injectDiv.innerHTML = injectCode; 
                        YAHOO.util.Dom.insertBefore(injectDiv, commentForm);
                
                        /* Refresh commentForm as we've added new forms to the page */
                        commentForm = _this.findCommentForm();

                        /* Setup event handler to enable/disable buttons on text input */
                        /*YAHOO.util.Event.addListener("comment-text", "keyup", atpComments.toggleButtons);
                        YAHOO.util.Event.addListener("comment-author", "keyup", atpComments.toggleButtons);
                        YAHOO.util.Event.addListener("comment-email", "keyup", atpComments.toggleButtons);*/
                        _this.setupButtonToggles();

                        /* Stop default submit event from firing */
                        YAHOO.util.Event.addListener(commentForm, 'submit', function ( e ) { 
                            YAHOO.util.Event.stopEvent( e )
                        });

                        /* If the commentForm already has a 'preview' element, attach the event handler */
                        if ( commentForm.preview ) {
                            commentForm.preview.setAttribute('onclick','');
                            YAHOO.util.Event.addListener(commentForm.preview, 'click', function () { 
                                atpComments.previewOnly( 1 ) 
                            });

                        /* No obvious preview element */
                        } else {
                            /* "Image" inputs can't accept event handlers, so convert it to a submit button */
                            YAHOO.util.Dom.getElementsBy( function ( el ) {
                                if ( el.name == 'preview' && el.type == 'image' ) {
                                    el.type = 'submit';
                                    el.disabled = false;
                                }
                            }, 'input', commentForm );                        

                            /* If we've found a preview element, setup an event handler for it */
                            if ( commentForm.preview ) {
                                YAHOO.util.Event.addListener( commentForm.preview, 'click', function () { 
                                    atpComments.previewOnly( 1 ) 
                                });
                            }
                        }
                    
                        if ( !commentForm.post ) {
                            /* "image" inputs can't accept event handlers. */
                            /* So, convert them to submit buttons */
                            YAHOO.util.Dom.getElementsBy( function ( el ) {
                                if ( el.name == 'post' && el.type == 'image' ) {
                                    el.type = 'submit';
                                    el.disabled = false;
                                }
                            }, 'input', commentForm );                        

                            /* If we still haven't found a "post" button, look */
                            /* for all of the inputs in the form. If there's only */
                            /* one input of type submit, assume it's a "post" button. */
                            if( !commentForm.post ) {
                                var inputs = YAHOO.util.Dom.getElementsBy( function ( el ) {
                                    return el.type == 'submit' ? true : false;
                                }, 'input', commentForm );
                                if ( inputs.length == 1 ) {
                                    commentForm.post = inputs[ 0 ]; 
                                }
                            }
                        }

                        /* Setup event handler for submitting a comment */
                        if( commentForm.post ) {
                            commentForm.post.setAttribute('onclick','');
                            YAHOO.util.Event.addListener(commentForm.post, 'click', function () { 
                                atpComments.interceptPost(1) 
                            });
                        } else {
                            /* The comment form has no submit button, we can't do anything */
                        }

                    /* Non advanced template case */
                    } else {
                        xid = _this.getEntryXid();

                        /* Setup event handler to enable/disable buttons on text input */
                        /*YAHOO.util.Event.addListener("comment-text", "keyup", atpComments.toggleButtons);
                        YAHOO.util.Event.addListener("comment-author", "keyup", atpComments.toggleButtons);
                        YAHOO.util.Event.addListener("comment-email", "keyup", atpComments.toggleButtons);*/
                        _this.setupButtonToggles();
                    }

                    /* If we have a preview confirmation div, and it's not hidden, ensure we've added stylesheet */
                    if( commentPreview = document.getElementById('comment-preview-confirmation') ) {
                        if( _this.getStyle(commentPreview,"display") != 'none' ) {
                            _this.addCommentStyles();
                        }
                    }

                    _this.params.loaded = 1;
                }
            });
        }
    },

    /* If the comment form input is blank, show it's default text in gray */
    /* Otherwise, the user enterd text should be black */
    commentInputStyling: function ( e ) {
        var el;
        var cInput;

        if ( !e ) { 
            var e = window.event;
        }

        if ( e.target ) {
            el = e.target;
        } else if ( e.srcElement ) {
            el = e.srcElement;
        }

        cInput = document.getElementById(el.id);

        if (e.type == 'focus') {
            cInput.className = '';
        } else if ((e.type == 'blur') && (el.value == el.getAttribute('at:default'))) {
            cInput.className = 'grayText';
        }       
    },

    /* Utility functions for handling cookies */
    cookie: {

        /* Return the value for a given cookie's name */
        get: function ( name ) { 
            var key = name + '=';
            var c = document.cookie;
            var i = c.indexOf(key);
            if (i < 0) return '';
            var j = c.indexOf(';', i + key.length);
            if (j < 0) j = c.length;
            var val = c.substring(i + key.length, j);

            try {
                val = decodeURIComponent(val);
            } catch (e) {
                /* ignore malformed URI sequence error.  bugzid:90422 */
                val = unescape(val);
            }

            return val;
        },

        /* Set a cookie with an expiration date */
        set: function ( name, value, expires ) { 
            document.cookie = name + '=' + encodeURIComponent(value) + 
                                '; expires=' + expires.toGMTString() + 
                                '; domain=' + hostName + 
                                '; path=/';
        },

        /* Remove a cookie, given it's name */
        remove: function () {
            if ( this.cookie.get(name) ) {
                this.cookie.set(name, '', new Date(70, 0, 1, 0, 0, 1));
            }
        }

    },

    /* Maintain lists of element visibility for various comment form states */
    formState: {
        defaultState: {
            "comments-open-login" : "none",
            "comments-open-logout" : "block",
            "comments-open-data" : "none",
            "comments-open-text" : "block",
            "comments-open-footer" : "block"
        },

        regOptional: {
            "comments-open-login" : "block",
            "comments-open-logout" : "none",
            "comments-open-data" : "block",
            "comments-open-text" : "block",
            "comments-open-footer" : "block"
        },

        regRequired: {
            "comments-open-login" : "block",
            "comments-open-logout" : "none",
            "comments-open-data" : "none",
            "comments-open-text" : "none",
            "comments-open-footer" : "none"
        },

        signInMessaging: {
            "comments-open-text" : "block",
            "comments-must-login-first" : "block"
        }
    },

    checkLocal: function () {
        /* See if commenterName variable is already set */
        if ( typeof this.params.commenterName != "undefined" && this.params.commenterName ) {
            return;
        }

        /* Set commenter cookie if we need to */
        var q = window.location.search.substring(1);
        var parms = q.split('&');
        for (var i = 0; i < parms.length; i++) {
            var pos = parms[i].indexOf('=');
            if (pos > 0) {
                var key = parms[i].substring(0,pos);

                if (key != "commenter") {
                    continue;
                }

                this.params.commenterName = decodeURIComponent(parms[i].substring(pos+1));

                if( typeof this.params.commenterName == "undefined" || !this.params.commenterName ) {
                    this.cookie.remove("commenter");
                } else {
                    var now = new Date();
                    now.setTime(now.getTime() + 60 * 60 * 1000);
                    this.cookie.set("commenter", this.params.commenterName, now);
                }
                return;
            }
        }

        /* Look for a cookie if its still not set */
        this.params.commenterName = this.cookie.get("commenter");
    },
    
    commentSignIn: function () {

        if ( typeof this.params.commenterName == "undefined" || !this.params.commenterName || !this.cookie.get("VID")) {
            var cForm = this.findCommentForm();
            var val;

            if(!cForm) {
                return false;
            }

            /* Populate author, email, and URL fields if we have the cookies */
            val = this.cookie.get("typepadauthor");
            if( cForm.author && val ) {
                cForm.author.value = val;
                cForm.author.className = '';
            }
            val = this.cookie.get("typepademail");
            if( cForm.email && val ) {        
                cForm.email.value = val;
                cForm.email.className = '';
            }
            val = this.cookie.get("typepadurl");
            if( cForm.url && val ) {
                cForm.url.value = val;
                cForm.url.className = '';
            }
            
            if( registrationRequired ){
                this.setElDisplayFor( this.formState.regRequired );

                /* Used in the microblog case where we want the commenter to sign in first */
                if( showSignInMessaging ) {
                    document.getElementById( 'comment-text' ).disabled = true;
                    this.setElDisplayFor( this.formState.signInMessaging );
                }
            } else if( registrationOptional ) {
                this.setElDisplayFor( this.formState.regOptional );
            }

            return;
        }
        
        // insert name
        var e = document.getElementById( "commenter-name" );
        var e2 = document.getElementById( "comment-preview-author" );
        if( e && e2 && this.cookie.get("VID") ) {
            e.innerHTML = this.params.commenterName;
            e2.innerHTML = this.params.commenterName;
        }
        
        // hide/show various elements
        this.setElDisplayFor( this.formState.defaultState );
    },

    /* Find the comment form within document.forms */
    findCommentForm: function () { 
        for ( var commentFormNum = 0; commentFormNum < document.forms.length; commentFormNum++ ) {
            if( document.forms[commentFormNum].id && (document.forms[commentFormNum].id == 'comment-form') ) {
                return document.forms[commentFormNum];
                break;
            }else if( document.forms[commentFormNum].getAttributeNode('action') && document.forms[commentFormNum].getAttributeNode('action').value.match("\/t\/comments$") ){
                return document.forms[commentFormNum];
                break;
            }else if( document.forms[commentFormNum].getAttributeNode('name') && document.forms[commentFormNum].getAttributeNode('name').value == "comments_form" ) {
                return document.forms[commentFormNum];
                break;
            }
        }
        return -1;
    },

    getEntryXid: function () {
        var form = this.findCommentForm();

        /* Return either the XID or the ID of the entry */
        if ( form.entry_xid ) {
            xid = form.entry_xid.value;
        } else {
            var typePadID = form.entry_id.value;
            var jsonClient = new TC.JSONRPCClient( jsonURL );
            var method = isPage ? 'Page.LookupByTypepadId' : 'Post.LookupByTypepadId';
            jsonClient.request( {
                method: method,
                params: {
                    typepad_id: typePadID
                },
                callback: function( r ) {
                    if ( r.error ) {
                        alert( r.error );
                    } else {
                        xid = r.result;
                    }
                }
            } );

        }
        return xid;
    },

    /* Set the CSS attribute display for each key/value pair in obj */
    setElDisplayFor: function ( obj ) {
        for ( var i in obj ) {
            e = document.getElementById( i );
            if( e ) {
                e.style.display = obj[ i ];
            }
        }            
    },

    /* This is called on page load to setup event handlers to enable/disable the comment form buttons.
       However, due to an inconsistent race condition, this function exists to setup an interval 
       to keep checking for the form elements in case they aren't there on the first attempt */
    setupButtonToggles: function() {
        var that = this;

        /* Check to see if the comment form elements exist on the page */
        if( document.getElementById('comment-text') &&
            document.getElementById('comment-author') &&
            document.getElementById('comment-email') ) {

                /* They do, so setup the event handlers and clear the interval (if it exists) */
                YAHOO.util.Event.addListener("comment-text", "keyup", atpComments.toggleButtons);
                YAHOO.util.Event.addListener("comment-author", "keyup", atpComments.toggleButtons);
                YAHOO.util.Event.addListener("comment-email", "keyup", atpComments.toggleButtons);

                if( this.params.buttonInterval ) {
                    clearInterval( this.params.buttonInterval );
                }

        /* The comment form elements weren't found. If the interval doesn't already exist, set it up */
        } else if( !this.params.buttonInterval ) {
            this.params.buttonInterval = setInterval( function() { 
                that.setupButtonToggles();
            }, 1000 );

        }
        /* Invisible else case: interval is doing it's thing, nothing to do */

    },

    init: function () {
        /* If registration is required or optional show the login form */
        if( registrationRequired || registrationOptional ){
            if( document.getElementById('comments-open-login') )
                document.getElementById('comments-open-login').style.display = 'block';
            /* Note: This is overriden if you're signed in at commentsignin */
        }


        this.checkLocal();
        this.commentSignIn();

        if ( registrationRequired && (typeof( this.params.commenterName ) == "undefined" || !this.params.commenterName || !this.cookie.get( 'VID' )) ) {
            /* We don't want to do anything if registration is required and there's no one logged in */
        } else {
            if ( document.getElementById('comments-open-text') ) {
                document.getElementById('comments-open-text').style.display = 'block';
            } 

            if ( document.getElementById('comments-open-data') ) {
                if ( !registrationRequired && !registrationOptional ) { /* no reg, always show */
                    document.getElementById('comments-open-data').style.display = 'block';
                } else if ( registrationRequired ) {
                    /* Reg required, never show */
                } else if ( registrationOptional ) {
                    /* Reg optional, only show if not logged in */
                    if ( (typeof( this.params.commenterName ) == "undefined") || !this.params.commenterName ) {
                        document.getElementById('comments-open-data').style.display = 'block';
                    }
                }
            }

            if ( document.getElementById('comments-open-footer') ) {
                document.getElementById('comments-open-footer').style.display = 'block';
            }

            /* If advanced templates: Set flag and prepare */
            for ( var i = 0; i < document.forms.length; i++) {

                if ( (document.forms[i].getAttributeNode('action') && document.forms[i].getAttributeNode('action').value.match("\/t\/comments$")) ||
                     (document.forms[i].getAttributeNode('name') && document.forms[i].getAttributeNode('name').value == "comments_form") ) {
                    this.params.isAdvanced = 1;
                    this.prepare(document.forms[i]);
                    break;
                }
            }

            /* Otherwise, not advanced templates. If we can find a form, prepare */
            if (!this.params.isAdvanced && 
                    document.getElementById('comment-form') && 
                    document.getElementById('comment-form').tagName == 'FORM') {

                this.prepare(document.getElementById('comment-form'));
            }
        }
    }
    
};

/* Begin preparation of ATP Comments! */
sixApart_atpComments.init();


