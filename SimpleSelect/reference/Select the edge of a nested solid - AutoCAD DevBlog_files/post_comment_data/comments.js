/*
// Necessary variables defined elsewhere. Noted here for reference
var captchaImgAttempts
var allowAnonComments
var jsonURL
var captchaSrc
var alertCommentCannotBeBlank
var alertAuthorCannotBeBlank
var alertEmailCannotBeBlank
var alertEmailIsInvalid
var alertUnauthorizedAction
var entryPermalink
var commentFooterLinks
var commentHeader
var commentReplyXID
var commentReplyAuthor
var commentReplyComplete
*/

tpe_comments = {
    is_loaded: true,
    previewOnly: function(e) {
        // e = 1 - comment-confirmation
        // e = 2 - only show comment form

        // If we're just showing form.. do only that
        if(e == 2) {
            // Hide preview comment, show form
            YAHOO.util.Dom.addClass('comment-preview', 'hiddenBox');
            YAHOO.util.Dom.addClass('header-preview-comment', 'hiddenBox');
            YAHOO.util.Dom.addClass('comment-preview-confirmation', 'hiddenBox');
            YAHOO.util.Dom.removeClass('comment-signin', 'hiddenBox');
            // if reply..
            if(comment_reply) {
                YAHOO.util.Dom.removeClass('primary-comment-form', 'hiddenBox');
                YAHOO.util.Dom.removeClass('comment-preview-confirmation', 'lessBoxMargin');
            } else
                YAHOO.util.Dom.removeClass('primary-comment-form', 'hiddenBox');
            //if(commentHeader)
            //    document.getElementById('comment-title').innerHTML = commentHeader;
            adjustiFrameHeight();
			
            return false;
        }

        var cForm = document.getElementById('comment-form');

        this.clearErrors();

        if(!this.validateInput()) 
            return false;

        tpe_comments.disableButtons();
        YAHOO.util.Dom.removeClass('commentEntryFormSpinner', 'hiddenBox');
        YAHOO.util.Dom.removeClass('previewFormSpinner', 'hiddenBox');
        YAHOO.util.Dom.addClass('comment-signin', 'hiddenBox');

        // JSON RPC Magic
        var jsonClient = new TC.JSONRPCClient( jsonURL );
        if( user_xid ) {
            var jsonParamValues = { 
                text: cForm.comment.value }        
        } else {
            var jsonParamValues = {
                author: cForm.author.value,
                email: cForm.email.value,
                url: cForm.url.value,
                text: cForm.comment.value }
        }
        
        jsonParamValues['entry_xid'] = xid; 

        if(comment_reply) 
            jsonParamValues['reply_xid'] = comment_xid;

        jsonClient.request( {
            method: 'Comments.Preview',
            params: jsonParamValues,
            callback: function( r ) {
                if( r.error ){
                    if( r.error == "ArcheType::Error::API::Feedback::Empty" ){
                        alert(alertCommentCannotBeBlank);
                    } else if( r.error = "Please enter a valid URL" ){
                        alert(r.error);
                    }
                    /*
                    else
                        tpe_comments.showErrorMessages(r.error);
                    // we shouldn't use this since it makes the iframe unpredictable heights
                    */
                }else{
                    tpe_comments.populateCommentPreview(r);
                    YAHOO.util.Dom.removeClass('comment-preview', 'hiddenBox');
                    YAHOO.util.Dom.removeClass('header-preview-comment', 'hiddenBox');

                    if(e == 1) {
                        YAHOO.util.Dom.removeClass('comment-preview-confirmation', 'hiddenBox');
                    }

                    if(comment_reply) {
                        //document.getElementById('comment-preview-replyto').innerHTML = " in reply to <a href=\"#\">"+commentReplyAuthor+"</a>";
                        YAHOO.util.Dom.addClass('comment-preview-confirmation', 'lessBoxMargin');
                        YAHOO.util.Dom.addClass('primary-comment-form', 'hiddenBox');
                    } else {
                        //document.getElementById('comment-preview-replyto').innerHTML = "";
                        YAHOO.util.Dom.addClass('primary-comment-form', 'hiddenBox');
                    }
                }

                tpe_comments.enableButtons();
                YAHOO.util.Dom.addClass('commentEntryFormSpinner', 'hiddenBox');
                YAHOO.util.Dom.addClass('previewFormSpinner', 'hiddenBox');
                adjustiFrameHeight();
            }
        } );
                
        if(e == 1) 
            return false;
        else
            return true;
    },
    interceptPost: function(recaptcha) {

        var cForm = document.getElementById('comment-form');
        this.clearErrors();

        // validation..
        if(!this.validateInput()) {
            return false;
        }

        var now = new Date();
        now.setTime(now.getTime() + 365 * 24 * 60 * 60 * 1000);
        if( cForm.author && cForm.author.value )
        	setCookie('typepadauthor', cForm.author.value, now);
        if( cForm.email && cForm.email.value )
        	setCookie('typepademail', cForm.email.value, now);
        if( cForm.url && cForm.url.value )
        	setCookie('typepadurl', cForm.url.value, now);
        
        
        tpe_comments.disableButtons();
        YAHOO.util.Dom.removeClass('commentEntryFormSpinner', 'hiddenBox');
        YAHOO.util.Dom.removeClass('previewFormSpinner', 'hiddenBox');
       
        // JSON Magic
        var jsonClient = new TC.JSONRPCClient( jsonURL );
        
        var jsonParamValues;
        var method;
        if( comment_edit ){
            jsonParamValues = {
                comment_xid: comment_xid,
                set: {
                    body: cForm.comment.value
                }
            };
            
            method = 'Comments.UpdateByCommenter';            
        }else{
            if( user_xid ){
                jsonParamValues = { 
                    text: cForm.comment.value }        
            }else{
                jsonParamValues = {
                    author: cForm.author.value,
                    email: cForm.email.value,
                    url: cForm.url.value,
                    text: cForm.comment.value }
            }

            if( comment_reply ) 
                jsonParamValues['reply_xid'] = comment_xid;

            method = 'Comments.Create';
        }
        jsonParamValues['entry_xid'] = xid;
        jsonParamValues['token'] = token;
        
        jsonClient.request( {
            method: method,
            params: jsonParamValues,
            callback: function( r ) {
                
                // in case we're coming from previewing a comment
                YAHOO.util.Dom.addClass('comment-preview-confirmation', 'hiddenBox'); 
                if ( r.error ) {
                    if(r.error == "ArcheType::Error::CaptchaRequired" || r.error == "ArcheType::Error::API::Feedback::Suspected" ){
                        if(recaptcha == 1) {
                            tpe_comments.generateReCaptcha(0);
                        }
                        else {
                            tpe_comments.generateCaptcha(0);
                        }
                    }else if(r.error == "TypeCore::Error::Unauthorized"){
                        alert(alertUnauthorizedAction);
                    }else if(r.error == "ArcheType::Error::API::Feedback::Empty"){
                        alert(alertCommentCannotBeBlank);
                    }else if(r.error == "EditNotPermitted"){
                        alert(alertEditingExpired);
                    }else if(r.error == "Name and email address are both required!"){
                        alert(alertEmailCannotBeBlank);
                    }else if(r.error == "We\'re sorry, but your session has expired. We cannot complete your request. Please refresh the page and try again."){
                        alert(alertTokenExpired);
                    }else{
                        YAHOO.util.Dom.removeClass('primary-comment-form', 'hiddenBox');
                        alert(r.error);
                    }
                    tpe_comments.enableButtons();
                    YAHOO.util.Dom.addClass('commentEntryFormSpinner', 'hiddenBox');
                    YAHOO.util.Dom.addClass('previewFormSpinner', 'hiddenBox');
                } else { 
                    tpe_comments.enableButtons();
                    YAHOO.util.Dom.addClass('commentEntryFormSpinner', 'hiddenBox');
                    YAHOO.util.Dom.addClass('previewFormSpinner', 'hiddenBox');
                    tpe_comments.commentComplete(r);
                }
            }
        } );

        return false;
    },
    submitWithCaptcha: function() {
        cForm = document.getElementById('comment-form');
        document.getElementById('captcha-continue').disabled = true;
        YAHOO.util.Dom.removeClass('captchaFormSpinner', 'hiddenBox');

        var jsonClient = new TC.JSONRPCClient( jsonURL );

        if( user_xid ) {
            var jsonParamValues = { 
                text: cForm.comment.value,
                captcha_code: document.getElementById('recaptcha_response_field').value,
                captcha_chal: document.getElementById('recaptcha_challenge_field').value }                   
        } else {
            var jsonParamValues = { 
                author: cForm.author.value,
                email: cForm.email.value,
                url: cForm.url.value,
                text: cForm.comment.value,
                captcha_code: document.getElementById('recaptcha_response_field').value,
                captcha_chal: document.getElementById('recaptcha_challenge_field').value }
        }

        jsonParamValues['entry_xid'] = xid; 

        if( comment_reply ) 
            jsonParamValues['reply_xid'] = comment_xid;
        jsonParamValues['token'] = token;

        jsonClient.request( {
            method: 'Comments.Create',
            params: jsonParamValues,
            callback: function( r ) {

                YAHOO.util.Dom.addClass('captchaFormSpinner', 'hiddenBox');
                document.getElementById('captcha-continue').disabled = false;

                if ( r.error ) {
                    if(r.error == "ArcheType::Error::CaptchaFailed") {
                        tpe_comments.generateCaptcha(1);
                    } else if(r.error == "ArcheType::Error::CaptchaRequired") {
                        tpe_comments.generateCaptcha(1);
                    } else {
                        /* alert(r.error); */
                        tpe_comments.showErrorMessages(r.error);
                    }
                } else {
                    tpe_comments.commentComplete(r);
                }
            }
        } );
    },
    generateCaptcha: function(failedAttempt) {	
        captchaImgAttempts++;
        if(failedAttempt) {
            YAHOO.util.Dom.addClass('comment-captcha', 'redBox');
            YAHOO.util.Dom.removeClass('comment-captcha', 'yellowBox');
            YAHOO.util.Dom.removeClass('captchaFailMsg', 'hiddenBox');
        }
        document.getElementById('comment-captcha-text').value = "";

        var jsonParamValues = { entry_xid: xid };

        var jsonCaptcha = new TC.JSONRPCClient( jsonURL );
        var data = jsonCaptcha.request( {
            method: 'Comments.CreateCaptcha',
            params: jsonParamValues,
            callback: function(r) {
                if(tpe_comments.previewOnly(3)) {

                    YAHOO.util.Dom.removeClass('comment-captcha', 'hiddenBox');
                    YAHOO.util.Dom.addClass('primary-comment-form', 'hiddenBox');
                }

                newCaptchaSrc = captchaSrc + r.result.code_encrypted;
                document.getElementById('captchaImg').src = newCaptchaSrc;
                document.getElementById('comment-captcha-chal').value = r.result.challenge;
                adjustiFrameHeight();
            }
        } );
        
        if(captchaImgAttempts > 2) {
            YAHOO.util.Dom.addClass('comment-captcha-viewalt', 'hiddenBox');
        }       
    },
    submitWithReCaptcha: function() {
        cForm = document.getElementById('comment-form');
        document.getElementById('captcha-continue').disabled = true;
        YAHOO.util.Dom.removeClass('captchaFormSpinner', 'hiddenBox');

        var jsonClient = new TC.JSONRPCClient( jsonURL );

        if( user_xid ) {
            var jsonParamValues = { 
                text: cForm.comment.value,
                captcha_code: document.getElementById('recaptcha_response_field').value,
                captcha_chal: document.getElementById('recaptcha_challenge_field').value }                   
        } else {
            var jsonParamValues = { 
                author: cForm.author.value,
                email: cForm.email.value,
                url: cForm.url.value,
                text: cForm.comment.value,
                captcha_code: document.getElementById('recaptcha_response_field').value,
                captcha_chal: document.getElementById('recaptcha_challenge_field').value }
        }

        jsonParamValues['entry_xid'] = xid; 

        if( comment_reply ) 
            jsonParamValues['reply_xid'] = comment_xid;
        jsonParamValues['token'] = token;
        jsonParamValues['captcha_type'] = 'recaptcha';

        jsonClient.request( {
            method: 'Comments.Create',
            params: jsonParamValues,
            callback: function( r ) {

                YAHOO.util.Dom.addClass('captchaFormSpinner', 'hiddenBox');
                document.getElementById('captcha-continue').disabled = false;

                if ( r.error ) {
                    if(r.error == "ArcheType::Error::CaptchaFailed") {
                        tpe_comments.generateReCaptcha(1);
                    } else if(r.error == "ArcheType::Error::CaptchaRequired") {
                        tpe_comments.generateReCaptcha(1);
                    } else {
                        /* alert(r.error); */
                        tpe_comments.showErrorMessages(r.error);
                    }
                } else {
                    tpe_comments.commentComplete(r);
                }
            }
        } );
    },
    generateReCaptcha: function(failedAttempt) {  
        captchaImgAttempts++;
        if(failedAttempt) {
            YAHOO.util.Dom.addClass('comment-captcha', 'redBox');
            YAHOO.util.Dom.removeClass('comment-captcha', 'yellowBox');
            YAHOO.util.Dom.removeClass('captchaFailMsg', 'hiddenBox');
        }

        if(tpe_comments.previewOnly(3)) {
            YAHOO.util.Dom.removeClass('comment-captcha', 'hiddenBox');
            YAHOO.util.Dom.addClass('primary-comment-form', 'hiddenBox');
        }


        Recaptcha.reload();
        
        if(captchaImgAttempts > 2) {
            YAHOO.util.Dom.addClass('comment-captcha-viewalt', 'hiddenBox');
        }       
    },
    validateInput: function() {
        var cForm = document.getElementById('comment-form');
        var cText = cForm.comment;
        
        // validate comment text
        if(cText.value == "" || cText.value.match(/^\s*$/i)) {
            alert(alertCommentCannotBeBlank);
            return false;
        }   

        // if logged in with typekey then front end author / email  validation isn't necessary
        if( user_xid )
            return true;
        //console.log('no user');

        var cAuthor = cForm.author;
        var cEmail = cForm.email;
        var cUrl = cForm.url;
        var validEmailForm = /^[\w\+.'-]+@[\w-]+\.[\w.-]+$/;

        // validate author
        if(this.checkForDefaultValue(cAuthor)) {
            alert(alertAuthorCannotBeBlank);
            YAHOO.util.Dom.addClass('comment-author', 'errorBG');
            YAHOO.util.Dom.removeClass('comment-author-error', 'hiddenBox');
            return false;
        }

        // validate email
        if(this.checkForDefaultValue(cEmail) && ! allowAnonComments) {
            YAHOO.util.Dom.addClass('comment-email', 'errorBG');
            alert(alertEmailCannotBeBlank);
            YAHOO.util.Dom.removeClass('comment-email-error', 'hiddenBox');     
            return false;
        } else if(this.checkForDefaultValue(cEmail) && allowAnonComments) {
            // set to blank
            cEmail.value = "";
        } else if((cEmail.value && ! cEmail.value.match(validEmailForm)) || (! allowAnonComments && cEmail.value == "")) {
            YAHOO.util.Dom.addClass('comment-email', 'errorBG');
            alert(alertEmailIsInvalid); 
            YAHOO.util.Dom.removeClass('comment-email-error', 'hiddenBox');         
            return false;
        }

        // validate URL
        if(this.checkForDefaultValue(cUrl)) {
            // set to blank
            cUrl.value = "";
        }

        return true;        
    },
    clearErrors: function() {
        YAHOO.util.Dom.removeClass('comment-author', 'errorBG');
        YAHOO.util.Dom.removeClass('comment-email', 'errorBG');
        YAHOO.util.Dom.removeClass('comment-url', 'errorBG');

        YAHOO.util.Dom.addClass('comment-author-error', 'hiddenBox');
        YAHOO.util.Dom.addClass('comment-email-error', 'hiddenBox');
        YAHOO.util.Dom.addClass('comment-url-error', 'hiddenBox');      

        YAHOO.util.Dom.addClass('comment-error', 'hiddenBox');

    },
    commentComplete: function(r) {	
        YAHOO.util.Dom.addClass('comment-preview', 'hiddenBox');
        //hide our comment form
        YAHOO.util.Dom.addClass('header-verify-comment', 'hiddenBox');
        YAHOO.util.Dom.addClass('header-preview-comment', 'hiddenBox');
        YAHOO.util.Dom.addClass('primary-comment-form', 'hiddenBox');
        YAHOO.util.Dom.addClass('comment-captcha', 'hiddenBox');
		
        /*
        tpe_comments.populateCommentPreview(r);

        document.getElementById('comment-text').value = "";
        document.getElementById('comment-captcha-text').value = "";

        tpe_comments.populateCommentPreview(r);
        */

        //tell our parent what just happened
        var pending = 0;
        var spam = 0;
        if( r.result.flags ){
            for( var i = 0; i < r.result.flags.length; i++ ){
                if( r.result.flags[ i ] == 'pending') pending = 1;
                if( r.result.flags[ i ] == 'spam') spam = 1;
            }
        }
        if( pending || spam ){
            parent.location = entryPermalink + "#tpe-action-pending-"+r.result.comment_xid;
        }else if( comment_reply ){
            parent.location = entryPermalink + "#tpe-action-replied-"+r.result.comment_xid;
        }else if( comment_edit ){
            parent.location = entryPermalink + "#tpe-action-edited-"+r.result.comment_xid;
        }else{
            parent.location = entryPermalink + "#tpe-action-posted-"+r.result.comment_xid;
        }
    },
    populateCommentPreview: function(r) {
        var previewAuthor = "";
        var cPreviewText = document.getElementById('comment-preview-content');

        // Set Comment Text
        cPreviewText.innerHTML = "<p>"+r.result.text+"</p>";

            if(r.result.url) {
                previewAuthor = '<a href="' + r.result.url + '">' + r.result.author + '</a>';
            } else {
                previewAuthor = r.result.author;
            }
        document.getElementById('comment-preview-author').innerHTML = previewAuthor;

        if(r.result.post_date) {
            document.getElementById('comment-preview-datetime').innerHTML = r.result.post_date;
        }       
        
        if(r.result.userpic) {
            document.getElementById('comment-preview-userpic').src = r.result.userpic;
        }
        
        // finally, show our previewed comment
        YAHOO.util.Dom.removeClass('comment-preview', 'hiddenBox');
    },
    postAnotherComment: function() {
       document.getElementById('comment-text').value = "";

        YAHOO.util.Dom.addClass('comment-complete', 'hiddenBox');
        YAHOO.util.Dom.removeClass('primary-comment-form', 'hiddenBox');        
    },
    checkForDefaultValue: function(o) {
        if((o.value ==  o.getAttribute('at:default')) || (o.value.match(/^\s*$/i))) {
            return true;
        }   
        return false;       
    },
    maxTextArea: function(limit_field, limit_num) {
        if( limit_field.value.length > limit_num )
            limit_field.value = limit_field.value.substring( 0, limit_num );
    },
    toggleButtons: function() {
        if( user_xid ){
            if(document.getElementById('comment-text').value.length >= 1)
                tpe_comments.enableButtons();
            else
                tpe_comments.disableButtons();
        }else{
            if( emailRequired ){
                if( document.getElementById('comment-text').value.length >= 1 && document.getElementById('comment-author').value != document.getElementById('comment-author').getAttribute('at:default') && document.getElementById('comment-author').value.length >= 1 && document.getElementById('comment-email').value != document.getElementById('comment-email').getAttribute('at:default') && document.getElementById('comment-email').value.length >= 1 )
                    tpe_comments.enableButtons();
                else
                    tpe_comments.disableButtons();                
            }else{
                if( document.getElementById('comment-text').value.length >= 1 && document.getElementById('comment-author').value != document.getElementById('comment-author').getAttribute('at:default') && document.getElementById('comment-author').value.length >= 1 )
                    tpe_comments.enableButtons();
                else
                    tpe_comments.disableButtons();                
            }
        }
    },
    enableButtons: function() {
        document.getElementById('comment-preview-button').disabled = false;
        document.getElementById('comment-post-button').disabled = false;        
    },
    disableButtons: function() {
        document.getElementById('comment-preview-button').disabled = true;
        document.getElementById('comment-post-button').disabled = true;     
    },
    showErrorMessages: function(errorMsg) {
        var splitErrorMsg = errorMsg.split("::");
        var lastIndex = splitErrorMsg.length;
        document.getElementById('comment-error-msg').innerHTML = splitErrorMsg[lastIndex-2] + " " + splitErrorMsg[lastIndex-1];        
        YAHOO.util.Dom.removeClass('comment-error', 'hiddenBox');
        YAHOO.util.Dom.addClass('comment-captcha', 'hiddenBox');
        if(comment_reply)
            YAHOO.util.Dom.removeClass('primary-comment-form', 'hiddenBox');
        else
            YAHOO.util.Dom.removeClass('primary-comment-form', 'hiddenBox');
        
        YAHOO.util.Dom.removeClass('primary-comment-form', 'hiddenBox');
    },
    resetCommentForm: function() {
        YAHOO.util.Dom.addClass('comment-preview', 'hiddenBox');
        YAHOO.util.Dom.addClass('comment-preview-confirmation', 'hiddenBox');
        YAHOO.util.Dom.addClass('comment-error', 'hiddenBox');
        YAHOO.util.Dom.addClass('comment-captcha', 'hiddenBox');
        YAHOO.util.Dom.removeClass('primary-comment-form', 'hiddenBox');
    }
 };

YAHOO.register("tpe_comments",
               tpe_comments,
               {version: "0.0.1", build: "1337"});
