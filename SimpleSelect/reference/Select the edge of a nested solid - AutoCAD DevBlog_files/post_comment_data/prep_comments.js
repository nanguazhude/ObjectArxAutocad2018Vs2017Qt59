var loaded = 0;

prepareTpeComments(document.getElementById('comment-form'));

function prepareTpeComments(form){
    startLoader();
  
    if(document.addEventListener){
        if( !user_xid ){
            form.author.addEventListener('focus',commentInputStyling,false);
            form.author.addEventListener('blur',commentInputStyling, false);
            form.email.addEventListener('focus',commentInputStyling,false);
            form.email.addEventListener('blur',commentInputStyling, false);
            form.url.addEventListener('focus',commentInputStyling,false);
            form.url.addEventListener('blur',commentInputStyling, false);
        }
        form.comment.addEventListener('keyup',commentInputStyling,false);
    } else if(document.attachEvent){
        if( !user_xid ){
            form.author.attachEvent('onfocus',commentInputStyling);
            form.author.attachEvent('onblur',commentInputStyling);
            form.email.attachEvent('onfocus',commentInputStyling);
            form.email.attachEvent('onblur',commentInputStyling);
            form.url.attachEvent('onfocus',commentInputStyling);
            form.url.attachEvent('onblur',commentInputStyling);
        }
        form.comment.attachEvent('onkeyup',commentInputStyling);
    }
    if( comment_reply )
        adjustiFrameHeight();
}

function adjustiFrameHeight() {
    var form_height = document.body.offsetHeight + 20;
    parent.location = entryPermalink + "#tpe-action-resize-"+form_height;
}

function startLoader() {
    if (!loaded) {
        var loader = new YAHOO.util.YUILoader();
        loader.addModule({
            name: "tpe_comments",
            type: "js",
            fullpath: atpCommentsPath
        });
        loader.addModule({
            name: "jsonrpc",
            type: "js",
            fullpath: "/.shared/js/jsonrpc-client.js"       
        });
        loader.require("yahoo-dom-event", "connection", "json", "jsonrpc", "tpe_comments");      
        loader.insert({
            onSuccess: function() {
                if( !user_xid ){
                    YAHOO.util.Event.addListener("comment-author", "keyup", toggleControl);
                    YAHOO.util.Event.addListener("comment-email", "keyup", toggleControl);
                }
                YAHOO.util.Event.addListener("comment-text", "keyup", toggleControl);
            }
        });
        loaded = 1;
    }
}

function toggleControl(){
    if( tpe_comments ){
        try{
            tpe_comments.toggleButtons();
        }catch(e){
            // window.console.log("toggle:",e);
        }
    }
}

function commentCancel( xid ){
    parent.location = entryPermalink + "#tpe-action-cancel-"+xid;
}

function commentInputStyling(e) {
    if( !e ) var e = window.event;
    var el;
    if( e.target )
        el = e.target;
    else if( e.srcElement )
        el = e.srcElement;
    var cInput = document.getElementById(el.id);
    if(e.type == 'focus') {
        cInput.className = '';
    } else if((e.type == 'blur') && (el.value == el.getAttribute('at:default'))) {
        cInput.className = 'grayText';
    }       
}

function setCookie (name, value, expires) {
    document.cookie = name + '=' + encodeURIComponent(value) + '; expires=' + expires.toGMTString() + '; domain=' + hostName + '; path=/';
}

function getCookie (name) {
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
        // ignore "malformed URI sequence error.  bugzid:90422
        val = unescape(val);
    }
    return val;
}

function deleteCookie (name) {
    if (getCookie(name))
        setCookie(name, '', new Date(70, 0, 1, 0, 0, 1));
}

/* comment registration */	
var showHideElements =
{
	"comments-open-login" : "none",
	"comments-open-logout" : "block",
	"comments-open-data" : "none",
	"comments-open-text" : "block",
	"comments-open-footer" : "block"
};

var showHideTkElements =
{
	"comments-open-login" : "block",
	"comments-open-logout" : "none",
	"comments-open-data" : "none",
	"comments-open-text" : "none",
	"comments-open-footer" : "none"
};

function commentSignIn(){
	if( typeof commenterName == "undefined" || !commenterName || !getCookie("VID")){
	    var cForm = document.getElementById('comment-form');
        if(!cForm)
            return false;
    	var val = getCookie("typepadauthor");
        if( cForm.author && val){
        	cForm.author.value = val;
        	cForm.author.className = '';
        }
        val = getCookie("typepademail");
        if( cForm.email && val){        
        	cForm.email.value = val;
        	cForm.email.className = '';
        }
        val = getCookie("typepadurl");
        if( cForm.url && val){
        	cForm.url.value = val;
        	cForm.url.className = '';
        }
        
        if( registrationRequired ){
        	for( var i in showHideTkElements )
        	{
        		e = document.getElementById( i );
        		if( e )
        			e.style.display = showHideTkElements[ i ];
        	}            
        }
		return;
	}
	
	// insert name
	var e = document.getElementById( "commenter-name" );
	var e2 = document.getElementById( "comment-preview-author" );
	if( e && e2 && getCookie("VID") ) {
		e.innerHTML = commenterName;
		e2.innerHTML = commenterName;
	}
	
	// hide/show various elements
	for( var i in showHideElements )
	{
		e = document.getElementById( i );
		if( e )
			e.style.display = showHideElements[ i ];
	}
}

commentSignIn();
