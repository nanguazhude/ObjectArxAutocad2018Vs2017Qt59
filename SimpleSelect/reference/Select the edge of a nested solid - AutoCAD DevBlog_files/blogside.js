if(typeof(TPConnect) == 'undefined')
    TPConnect = {};

TPConnect.blogside = {
    init: function(){ // runs after everything else on this page is loaded, iframe exists, comments may not!
        setInterval( function(){ TPConnect.blogside.poll() }, 500 );

        YAHOO.util.Event.on('tpc-flyout','mouseout',TPConnect.blogside.hideFlyout );

        this.timeoutEdits();
    },
    reply: function( xid ){ // this will reposition and reload the iframe under the comment you want to reply to
        TPConnect.iframe.parentNode.removeChild( TPConnect.iframe );// I love duplication!
        TPConnect.signin.parentNode.removeChild( TPConnect.signin );// I love duplication!
        var old_xid;
        if( old_xid = TPConnect.iframe.getAttribute('tpc:current-xid') ){
            // if the iframe has the current-xid attribute then it's already a reply or an edit and we need to reset the state of that reply/edit
            YAHOO.util.Dom.removeClass('comment-'+old_xid+'-reply', 'hiddenBox');
            YAHOO.util.Dom.removeClass('comment-'+old_xid, 'hiddenBox');
        }
        
        TPConnect.iframe.src = TPConnect.app_url + 'connect/blogs/' + TPConnect.blog_xid + '/post_comment?asset_id=' + TPConnect.post_xid + '&comment_xid=' + xid +'&mode=reply&color=' + encodeURIComponent(TPConnect.text_color) + '&acolor=' + encodeURIComponent(TPConnect.text_acolor) + '&width=' + encodeURIComponent(TPConnect.width - 81);
        TPConnect.iframe.width = (TPConnect.width - 75);
        TPConnect.iframe.setAttribute('tpc:current-xid', xid);
        
        // dumb indentation
        YAHOO.util.Dom.addClass(TPConnect.iframe, 'iframe-reply');
        YAHOO.util.Dom.addClass(TPConnect.signin, 'signin-reply');
        
        // remove reply link from replyee since we're replying
        YAHOO.util.Dom.addClass('comment-'+xid+'-reply', 'hiddenBox');
        
        var replyee = document.getElementById('comment-'+xid);
        
        TPConnect.iframe.id = 'comments-post-iframe-'+xid;
        YAHOO.util.Dom.insertAfter(TPConnect.iframe, replyee);
        YAHOO.util.Dom.insertAfter(TPConnect.signin, replyee);
    },
    edit: function( xid ){ // similar to reply, this repositions and reloads the iframe when editing
        // completely hide the comment we're editing
        YAHOO.util.Dom.addClass('comment-'+xid, 'hiddenBox');
        
        TPConnect.iframe.parentNode.removeChild( TPConnect.iframe );
        TPConnect.signin.parentNode.removeChild( TPConnect.signin );
        var old_xid;
        if( old_xid = TPConnect.iframe.getAttribute('tpc:current-xid') ){
            // if the iframe has the current-xid attribute then it's already a reply or an edit and we need to reset the state of that reply/edit
            YAHOO.util.Dom.removeClass('comment-'+old_xid+'-reply', 'hiddenBox');
            YAHOO.util.Dom.removeClass('comment-'+old_xid, 'hiddenBox');
        }
        
        TPConnect.iframe.src = TPConnect.app_url + 'connect/blogs/' + TPConnect.blog_xid + '/post_comment?asset_id=' + TPConnect.post_xid + '&comment_xid=' + xid +'&mode=edit&color=' + encodeURIComponent(TPConnect.text_color) + '&acolor=' + encodeURIComponent(TPConnect.text_acolor);
        TPConnect.iframe.setAttribute('tpc:current-xid', xid);

        var editee = document.getElementById('comment-'+xid);
        
        TPConnect.iframe.id = 'comments-post-iframe-'+xid;
        YAHOO.util.Dom.insertAfter(TPConnect.iframe, editee);
        YAHOO.util.Dom.insertAfter(TPConnect.signin, editee);
    },
    timeoutEdits: function(){
        if( YAHOO.util.Dom.get('tpe-comments-content') ) {
            var editable_comments = YAHOO.util.Dom.getChildrenBy( YAHOO.util.Dom.get('tpe-comments-content'), function( el ){ if( YAHOO.util.Dom.hasClass( el, 'comment' ) && el.getAttribute('tpc:expires') ) return true; } );
            for( var i = 0; i < editable_comments.length; i++){
                var expireTime = Date.parse( editable_comments[ i ].getAttribute('tpc:expires') );
                editable_comments[ i ].removeAttribute('tpc:expires');
                var curTime = new Date();

                // SUDO make me a GMT date object
                var minutes = curTime.getMinutes();
                curTime.setMinutes( minutes + curTime.getTimezoneOffset() );

                var mili = expireTime - curTime;
                var xid = editable_comments[ i ].id.substr(8);
                setTimeout( function(){ TPConnect.blogside.removeEdit( xid ) }, mili );
            }
        }
    },
    removeEdit: function( xid ){
        YAHOO.util.Dom.addClass('comment-'+xid+'-edit', 'hiddenBox');
        YAHOO.util.Dom.addClass('comment-'+xid+'-delete', 'hiddenBox');
        YAHOO.util.Dom.removeClass('comment-'+xid+'-editexpired', 'hiddenBox');
    },
    resetiFrame: function(){
        TPConnect.iframe.parentNode.removeChild( TPConnect.iframe );
        TPConnect.signin.parentNode.removeChild( TPConnect.signin );
        TPConnect.iframe.src = TPConnect.app_url + 'connect/blogs/' + TPConnect.blog_xid + '/post_comment?asset_id=' + TPConnect.post_xid + '&color=' + encodeURIComponent(TPConnect.text_color) + '&acolor=' + encodeURIComponent(TPConnect.text_acolor) + '&width=' + encodeURIComponent(TPConnect.width - 6);
        if( TPConnect.iframe.getAttribute('tpc:current-xid') ){
            YAHOO.util.Dom.removeClass('comment-'+TPConnect.iframe.getAttribute('tpc:current-xid')+'-reply', 'hiddenBox');
            TPConnect.iframe.removeAttribute('tpc:current-xid');
        }
        
        YAHOO.util.Dom.removeClass(TPConnect.iframe, 'iframe-reply');
        YAHOO.util.Dom.removeClass(TPConnect.signin, 'signin-reply');
        TPConnect.iframe.width = TPConnect.width;

        TPConnect.iframe.id = 'comments-post-iframe';
        YAHOO.util.Dom.get('comments-post').appendChild( TPConnect.signin );
        YAHOO.util.Dom.get('comments-post').appendChild( TPConnect.iframe );
    },
	scrollTo: function(){
		if( TPConnect.iframe && TPConnect.iframe.getAttribute('tpc:scroll-xid') ){
			var xid = TPConnect.iframe.getAttribute('tpc:scroll-xid');
			TPConnect.iframe.removeAttribute('tpc:scroll-xid');
			document.location.hash = '#comment-'+xid;
        }
	},
    commentAction: function( action, xid ){
        var confirmtext = "Are you sure you want to take this action?";
        if( action == 'delete' ){
            confirmtext = "Are you sure you want to delete this comment?";
        }

        if( confirm(confirmtext) ){
            TPConnect.actioniFrame.id = 'comments-action-iframe-'+xid;
            TPConnect.actioniFrame.src = TPConnect.app_url + 'connect/blogs/' + TPConnect.blog_xid + '/comment_action?mode=' + action + '&asset_id='+ TPConnect.post_xid +'&comment_xid=' + xid;            
        }
    },
    updateContact: function( action, xid ){
        TPConnect.actioniFrame.id = 'user-action-iframe-'+xid;
        if( TPConnect.post_xid ) 
            TPConnect.actioniFrame.src = TPConnect.app_url + 'services/connect/contact_embed?mode=' + action + '&asset_id=' + TPConnect.post_xid + '&other_user_id='+ xid;
        else 
            TPConnect.actioniFrame.src = TPConnect.app_url + 'services/connect/contact_embed?mode=' + action + '&other_user_id='+ xid;
    },
    prepFlyout: function( xid ){
        TPConnect.abortedFlyout = null;
        var timeoutID = setTimeout( function(){ TPConnect.blogside.showFlyout( xid ) }, 700);
        YAHOO.util.Event.addListener('comment-'+xid+'-avatarimg', 'mouseout', function(){ TPConnect.blogside.abortFlyout( xid, timeoutID ) } );
    },
    abortFlyout: function( xid, timeoutID ){
        TPConnect.abortedFlyout = 1;
        clearTimeout( timeoutID );
        YAHOO.util.Event.removeListener('comment-'+xid+'-avatarimg', 'mouseout');
    },    
    showFlyout: function( xid ){
        var userXid, flyoutType, src, tpc_blogside_flyout;

        if( document.getElementById('comment-'+xid+'-avatarimg').getAttribute('tpc:userxid') ){
            userXid = document.getElementById('comment-'+xid+'-avatarimg').getAttribute('tpc:userxid');
        }else{
            // WHOS USER IS THIS ANYWAYS?
            return false;
        }
        
        if(document.getElementById('comment-'+xid+'-avatarimg').getAttribute('tpc:flyout-type')) {
            flyoutType = document.getElementById('comment-'+xid+'-avatarimg').getAttribute('tpc:flyout-type');
        }

        // generate iFrame source
        src = TPConnect.app_url + 'services/connect/flyout?user_id=' + userXid + '&blog_id=' + TPConnect.blog_xid + '&flyout_type=' + flyoutType;
        
        // if not cached cache it
        if(!TPConnect.seenIt[userXid+flyoutType]) {
            // create a replacement iframe
            tpc_blogside_flyout = document.createElement('iframe');
            tpc_blogside_flyout.id = "tpc-flyout";
            tpc_blogside_flyout.className = "hiddenBox tpc-flyout tpc-flyout-" + (flyoutType || "blogside-default");
            tpc_blogside_flyout.scrolling = "no";
            tpc_blogside_flyout.frameBorder = "0";
            tpc_blogside_flyout.border = "0";
            tpc_blogside_flyout.src = src;
            tpc_blogside_flyout.allowtransparency = "true";
            TPConnect.seenIt[userXid+flyoutType] = tpc_blogside_flyout;
        }
        
        // replace existing iframe with cached iframe
        document.getElementById('tpc-flyout').parentNode.replaceChild(TPConnect.seenIt[userXid+flyoutType], document.getElementById('tpc-flyout')); 

        // add mouseout handler
        YAHOO.util.Event.on('tpc-flyout','mouseout',TPConnect.blogside.hideFlyout );

        // remove the abortFlyout handler
        YAHOO.util.Event.removeListener('comment-'+xid+'-avatarimg', 'mouseout');

        // show the flyout
        YAHOO.util.Dom.removeClass('tpc-flyout', 'hiddenBox');

        // set the flyouts position
        var pos = YAHOO.util.Dom.getXY('comment-'+xid+'-avatarimg');
        pos[0] = pos[0]-15;
        pos[1] = pos[1]-15;

        YAHOO.util.Dom.setXY('tpc-flyout', pos);

    },
    hideFlyout: function( e ){
        if( !YAHOO.util.Dom.isAncestor( 'tpc-flyout', YAHOO.util.Event.getRelatedTarget( e ) ) ){
            TPConnect.abortedFlyout = null;
            YAHOO.util.Dom.addClass('tpc-flyout', 'hiddenBox');
            window.focus();
        }
    },
    poll: function(){
        if( document.location.hash.substr(1) != TPConnect.oldhash ){
            if( document.location.hash.substr(1).match(/^tpe-action/) )
                this.hashAction( document.location.hash.substr(12) );
            TPConnect.oldhash = document.location.hash.substr(1);
        }
    },
    hashAction: function( hash ){
        var hashparts = hash.split('-');

        var xid = hashparts[ 1 ];
        if( hashparts[ 0 ] == 'resize' ){
            var height = xid; // obviously this isn't an xid in this case
        }else{
            var comment = YAHOO.util.Dom.get('comment-'+xid);			
        }

        switch( hashparts[ 0 ] ){
            case 'resize':
                TPConnect.iframe.height = height;
                this.scrollTo();
                break;
            case 'posted':
                this.appendPage(TPConnect.lowest_comment, TPConnect.newest_comment, xid);
                this.resetiFrame();
				TPConnect.iframe.setAttribute('tpc:scroll-xid', xid);
                //document.location.hash = '#comment-'+xid;
                break;
            case 'replied':
                this.resetiFrame();
                this.appendPage(TPConnect.lowest_comment, TPConnect.newest_comment, xid);
                TPConnect.iframe.setAttribute('tpc:scroll-xid', xid);
                break;
            case 'edited':
                this.resetiFrame();
                this.appendPage(TPConnect.lowest_comment, TPConnect.newest_comment, xid);
                TPConnect.iframe.setAttribute('tpc:scroll-xid', xid);
                break;
            case 'pending':
                this.resetiFrame();
                this.appendPage(TPConnect.lowest_comment, TPConnect.newest_comment, xid);
                TPConnect.iframe.setAttribute('tpc:scroll-xid', xid);
                this.scrollTo();
                break;
            case 'deleted':
                this.appendPage(TPConnect.lowest_comment, TPConnect.newest_comment, xid);
                TPConnect.iframe.setAttribute('tpc:scroll-xid', xid);
                break;
            case 'cancel':
                YAHOO.util.Dom.removeClass('comment-'+xid, 'hiddenBox');
                this.resetiFrame();
                TPConnect.iframe.setAttribute('tpc:scroll-xid', xid);
                this.scrollTo();
                break;
            case 'follow':
                YAHOO.util.Dom.removeClass('unfollow-action', 'hiddenBox');
                YAHOO.util.Dom.addClass('follow-action', 'hiddenBox');
                if(TPConnect.seenIt[ xid ])
                    TPConnect.seenIt[ xid ].is_contact = 1;
                break;
            case 'unfollow':
                YAHOO.util.Dom.removeClass('follow-action', 'hiddenBox');
                YAHOO.util.Dom.addClass('unfollow-action', 'hiddenBox');
                if(TPConnect.seenIt[ xid ])
                    TPConnect.seenIt[ xid ].is_contact = 0;
                break;
            default:
                //alert('unrecognized action');
                break;
        }
    },
    appendPage: function( lowest_comment, newest_comment, posted_comment ){
        //window.console.log("appending with low: "+lowest_comment+" and new: "+newest_comment);
        var script = document.createElement('script');
        var d = new Date();
        var is_post_action = 0;
        if ( typeof(posted_comment) == 'undefined' ) {
            posted_comment = '';
        }
            
        script.type = 'text/javascript';
        script.src = TPConnect.embed_src
                       + '?asset_id=' + TPConnect.post_xid
                       + '&d=1'
                       + '&p=1'
                       + '&dc=' + (is_post_action ? '1' : '0')
                       + '&posted_comment='  + posted_comment
                       + '&lowest_comment='  + lowest_comment
                       + '&newest_comment='  + newest_comment
                       + '&ts='              + d.getTime();

        YAHOO.util.Dom.get('tpe-content').appendChild(script);
    },
    reclassComments: function(){
        var comments = YAHOO.util.Dom.getChildrenBy( YAHOO.util.Dom.get('tpe-comments-content'), function( el ){ if( YAHOO.util.Dom.hasClass( el, 'comment' ) ) return true; } );
        
        for( var i = 0; i < comments.length; i++ ){
            var comment_number = YAHOO.util.Dom.getFirstChildBy( comments[ i ], function( el ){ if( el.className == 'comment-number' ) return 1; } );
            if( comment_number )
                comment_number.innerHTML = i+1;

            var classes = comments[ i ].className;
            classes = classes.replace(/even/,'').replace(/first/,'').replace(/last/,'');
            
            var first = 0;
            if( i == 0 )
                first = 1;
            var even = 0;
            if( i % 2 )
                even = 1;
            var last = 0;
            if( (i+1) == comments.length )
                last = 1;
            
            if( first )
                classes += ' first';
            if( even )
                classes += ' even';
            if( last )
                classes += ' last';

            comments[ i ].className = classes;
        }
    },
    getStyle: function(el, property){
        if( el.currentStyle )
            var style = el.currentStyle[ property ];
        else if( window.getComputedStyle )
            var style = window.getComputedStyle( el,null ).getPropertyValue( property );
        return style;
    }
};

TPConnect.signin = document.getElementById('comments-signin');
TPConnect.iframe = document.getElementById('comments-post-iframe');
TPConnect.actioniFrame = document.getElementById('action-iframe');
TPConnect.oldhash = document.location.hash.substr(1);
TPConnect.abortedFlyout = null;
TPConnect.flyerTimeoutID;
TPConnect.seenIt = new Object();
TPConnect.seenIt[ '0' ] = {name: 'empty user'}; // safari is not a fan of checking for things in empty objects

// we check for YUI before loading anything - this is mostly an issue on slower connections
function checkYUI(){
    if( typeof(YAHOO) == 'undefined' ){
        setTimeout( checkYUI, 2000);
    }else{
        TPConnect.blogside.init();
    }
}
checkYUI();
