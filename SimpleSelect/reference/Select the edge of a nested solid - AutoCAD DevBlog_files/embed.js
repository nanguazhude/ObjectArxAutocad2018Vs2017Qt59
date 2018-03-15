TPConnect.app_url   = 'http://www.typepad.com/';
//http://profile.typepad.com/
TPConnect.embed_src = 'http://profile.typepad.com/services/embed/tpc/6a0167607c2431970b0162ffdc445e970d/thread.js';
TPConnect.blog_xid  = '6a0167607c2431970b0162ffdc445e970d';
TPConnect.post_xid = '6a0167607c2431970b017d4099ad94970c';
TPConnect.user_xid = '';

//isolate our trivial js
( function() {
var tpe_url = TPConnect.tpc_url;
var tpe_title = TPConnect.tpc_title;
var tpe_message = TPConnect.tpc_message;
    
/* debug ==> 0 */

var tpe_referrer_uri = 'http://adndevblog.typepad.com/autocad/2013/01/select-the-edge-of-a-nested-solid.html';
var tpe_signin_uri = 'http://www.typepad.com/sitelogin?uri=http%3A%2F%2Fadndevblog.typepad.com%2Fautocad%2F2013%2F01%2Fselect-the-edge-of-a-nested-solid.html&fp=2e7ade01d713e55d1afef8cb1cbf25c8&view_uri=http%3A%2F%2Fprofile.typepad.com%2F&via=profile';
var tpe_host_url = 'http://profile.typepad.com/'; // can both these be in the same form?
var tpe_user_xid = '';
var tpe_sort = '';

//console.log("user xid = %s", tpe_user_xid);
//console.log("blog xid = %s", TPConnect.blog_xid);
//console.log("post xid = %s", TPConnect.post_xid);

////// Set up all query parameters

var tpe_href = window.location.href;
var tpe_date = new Date();
var tpe_hashIndex = tpe_href.indexOf('#');
if(tpe_hashIndex != -1) {
        tpe_href = tpe_href.substring(0, tpe_hashIndex);
}

if(typeof(tpe_url) == 'undefined') {
    tpe_url = tpe_href;
}
if(typeof(tpe_title) == 'undefined') {
    tpe_title = document.title || '';
}

if(typeof(tpe_message) == 'undefined') {
    tpe_message = '';
} else {
    var tpe_isUTF8 = false;
    if(/msie/i.test(navigator.userAgent) && !/opera/i.test(navigator.userAgent)) { // if IE
            for(var i=0; i<tpe_message.length; i++) {
                    if(tpe_message.charCodeAt(i) > 256) {
                            tpe_isUTF8 = true;
                            break;
                    }
            }               
    }
    if(tpe_isUTF8) {
            tpe_message = '';
    } else {
            if(tpe_message.length > 400) {
                    tpe_message = tpe_message.substring(0, tpe_message.indexOf(' ', 350));
            }
            // for CJK posts (no whitespaces between words)
            if(tpe_message.length > 400) {
                    tpe_message = tpe_message.substring(0, 400);
            }
    }
}

if(typeof(tpe_show_count) == 'undefined') {
        tpe_show_count = -1;
}
if(typeof(tpe_sort) == 'undefined') {
        tpe_sort = '';
}
if(typeof(tpe_show_threshold) == 'undefined') {
        tpe_show_threshold = 0;
}
if(typeof(tpe_container_id) == 'undefined') {
        tpe_container_id = 'tpc_thread';
}
if(typeof(tpe_category_id) == 'undefined') {
        tpe_category_id = '';
}
if(typeof(tpe_developer) == 'undefined') {
        tpe_developer = '';
}
if(typeof(tpe_iframe_css) == 'undefined') {
        tpe_iframe_css = '';
}

//<link rel="stylesheet" href="http://" type="text/css" media="screen" />
var tpe_style = document.createElement('link');
tpe_style.setAttribute('rel','stylesheet');
tpe_style.href = 'http://static.typepad.com/.shared:v6bc83ba:typepad:zh_cn/css/connect/typepad/screen.css';
tpe_style.type = 'text/css';
tpe_style.media = 'screen';
document.getElementsByTagName('head')[0].appendChild(tpe_style);

//<link rel="stylesheet" href="http://" type="text/css" media="screen" />
var tpe_style = document.createElement('link');
tpe_style.setAttribute('rel','stylesheet');
tpe_style.href = 'http://static.typepad.com/.shared:v6bc83ba:typepad:zh_cn/css/flyout-types.css';
tpe_style.type = 'text/css';
tpe_style.media = 'screen';
document.getElementsByTagName('head')[0].appendChild(tpe_style);

///// Setup some important divs that we'll use to build and grow the document
/// One DIV to hold them ALL
//<div id="tpe-content"></div>
var tpe_dataContainer = document.createElement('div');
tpe_dataContainer.id = 'tpe-content';
document.getElementById(tpe_container_id).appendChild(tpe_dataContainer);

//<div id="comments-loading"><img src="/.shared/images/spinner.gif" /></div>
var tpe_comments_loading = document.createElement('div');
tpe_comments_loading.id = 'comments-loading';
tpe_dataContainer.appendChild(tpe_comments_loading);

//<div class="comments-content" id="tpe-comments-content"></div>
var tpe_contentContainer = document.createElement('div');
tpe_contentContainer.id = 'tpe-comments-content';
tpe_dataContainer.appendChild(tpe_contentContainer);

//<div class="comments-pager" id="comments-pager"></div>
var tpe_comments_pager = document.createElement('div');
tpe_comments_pager.id = 'comments-pager';
tpe_dataContainer.appendChild(tpe_comments_pager);

//<div class="comments-signin font-entrybody" id="comments-signin"></div>
var tpe_comments_signin = document.createElement('div');
tpe_comments_signin.id = 'comments-signin';
tpe_comments_signin.innerHTML = '\n\n    \n<div class=\"login-list\">\n    \n    <span class=\"first\">Comment below or sign in with<\/span> <span class=\"typepad\" style=\"background:url(http://static.typepad.com/.shared:v6bc83ba:typepad:zh_cn/images/profile/service_icons/typepad.png) top left no-repeat;padding-left:20px;\"><a href=\"http://www.typepad.com/sitelogin?uri=http%3A%2F%2Fadndevblog.typepad.com%2Fautocad%2F2013%2F01%2Fselect-the-edge-of-a-nested-solid.html&fp=2e7ade01d713e55d1afef8cb1cbf25c8&view_uri=http%3A%2F%2Fprofile.typepad.com%2F&via=blogside&post_uri=http://adndevblog.typepad.com/autocad/2013/01/select-the-edge-of-a-nested-solid.html\">Typepad<\/a><\/span> <span class=\"facebook\" style=\"background:url(http://static.typepad.com/.shared:v6bc83ba:typepad:zh_cn/images/profile/service_icons/facebook.png) top left no-repeat;padding-left:20px;\"><a href=\"http://www.typepad.com/sitelogin?uri=http%3A%2F%2Fadndevblog.typepad.com%2Fautocad%2F2013%2F01%2Fselect-the-edge-of-a-nested-solid.html&fp=2e7ade01d713e55d1afef8cb1cbf25c8&view_uri=http%3A%2F%2Fprofile.typepad.com%2F&via=blogside&service=facebook&post_uri=http://adndevblog.typepad.com/autocad/2013/01/select-the-edge-of-a-nested-solid.html\">Facebook<\/a><\/span> <span class=\"twitter\" style=\"background:url(http://static.typepad.com/.shared:v6bc83ba:typepad:zh_cn/images/profile/service_icons/twitter.png) top left no-repeat;padding-left:20px;\"><a href=\"http://www.typepad.com/sitelogin?uri=http%3A%2F%2Fadndevblog.typepad.com%2Fautocad%2F2013%2F01%2Fselect-the-edge-of-a-nested-solid.html&fp=2e7ade01d713e55d1afef8cb1cbf25c8&view_uri=http%3A%2F%2Fprofile.typepad.com%2F&via=blogside&service=twitter&post_uri=http://adndevblog.typepad.com/autocad/2013/01/select-the-edge-of-a-nested-solid.html\">Twitter<\/a><\/span> <span class=\"googleplus\" style=\"background:url(http://static.typepad.com/.shared:v6bc83ba:typepad:zh_cn/images/profile/service_icons/googleplus.png) top left no-repeat;padding-left:20px;\"><a href=\"http://www.typepad.com/sitelogin?uri=http%3A%2F%2Fadndevblog.typepad.com%2Fautocad%2F2013%2F01%2Fselect-the-edge-of-a-nested-solid.html&fp=2e7ade01d713e55d1afef8cb1cbf25c8&view_uri=http%3A%2F%2Fprofile.typepad.com%2F&via=blogside&service=gplus&post_uri=http://adndevblog.typepad.com/autocad/2013/01/select-the-edge-of-a-nested-solid.html\">Google+<\/a><\/span>   <span>and <a href=\"http://www.typepad.com/sitelogin?uri=http%3A%2F%2Fadndevblog.typepad.com%2Fautocad%2F2013%2F01%2Fselect-the-edge-of-a-nested-solid.html&fp=2e7ade01d713e55d1afef8cb1cbf25c8&view_uri=http%3A%2F%2Fprofile.typepad.com%2F&via=blogside&service=openid&post_uri=http://adndevblog.typepad.com/autocad/2013/01/select-the-edge-of-a-nested-solid.html\">more...<\/a><\/span>\n    \n<\/div>\n    \n\n';
tpe_dataContainer.appendChild(tpe_comments_signin);

//<div class="comments-flyout" id="comments-flyout"></div>
var tpe_comments_flyout = document.createElement('div');
tpe_comments_flyout.id = 'comments-flyout';
tpe_dataContainer.appendChild(tpe_comments_flyout);

//<iframe id="tpc-flyout" src="http://static.typepad.com/.shared:v6bc83ba:typepad:zh_cn//services/connect/flyout?user_id=xxx"></iframe>
var tpe_blogside_flyout = document.createElement('iframe');
tpe_blogside_flyout.id = "tpc-flyout";
tpe_blogside_flyout.className = "tpc-flyout hiddenBox";
tpe_blogside_flyout.scrolling = "no";
tpe_blogside_flyout.frameBorder = "0";
tpe_blogside_flyout.border = "0";
tpe_blogside_flyout.allowtransparency = "true";
tpe_comments_flyout.appendChild(tpe_blogside_flyout);





function getStyle(el, property){
    if( el.currentStyle )
        var style = el.currentStyle[ property ];
    else if( window.getComputedStyle )
        var style = window.getComputedStyle( el,null ).getPropertyValue( property );
    return style;
}

/* some trimmed down code based here: http://www.phpied.com/rgb-color-parser-in-javascript/ */
function RGBColor(color_string)
{
    this.ok = false;

    // strip any leading #
    if (color_string.charAt(0) == '#') { // remove # if any
        color_string = color_string.substr(1,6);
    }

    color_string = color_string.replace(/ /g,'');
    color_string = color_string.toLowerCase();

    // array of color definition objects
    var color_defs = [
        {
            re: /^rgb\((\d{1,3}),\s*(\d{1,3}),\s*(\d{1,3})\)$/,
            example: ['rgb(123, 234, 45)', 'rgb(255,234,245)'],
            process: function (bits){
                return [
                    parseInt(bits[1]),
                    parseInt(bits[2]),
                    parseInt(bits[3])
                ];
            }
        },
        {
            re: /^(\w{2})(\w{2})(\w{2})$/,
            example: ['#00ff00', '336699'],
            process: function (bits){
                return [
                    parseInt(bits[1], 16),
                    parseInt(bits[2], 16),
                    parseInt(bits[3], 16)
                ];
            }
        },
        {
            re: /^(\w{1})(\w{1})(\w{1})$/,
            example: ['#fb0', 'f0f'],
            process: function (bits){
                return [
                    parseInt(bits[1] + bits[1], 16),
                    parseInt(bits[2] + bits[2], 16),
                    parseInt(bits[3] + bits[3], 16)
                ];
            }
        }
    ];

    // search through the definitions to find a match
    for (var i = 0; i < color_defs.length; i++) {
        var re = color_defs[i].re;
        var processor = color_defs[i].process;
        var bits = re.exec(color_string);
        if (bits) {
            channels = processor(bits);
            this.r = channels[0];
            this.g = channels[1];
            this.b = channels[2];
            this.ok = true;
        }
    }

    // validate/cleanup values
    this.r = (this.r < 0 || isNaN(this.r)) ? 0 : ((this.r > 255) ? 255 : this.r);
    this.g = (this.g < 0 || isNaN(this.g)) ? 0 : ((this.g > 255) ? 255 : this.g);
    this.b = (this.b < 0 || isNaN(this.b)) ? 0 : ((this.b > 255) ? 255 : this.b);

    // some getters
    this.toRGB = function () {
        return 'rgb(' + this.r + ', ' + this.g + ', ' + this.b + ')';
    }
    this.toHex = function () {
        var r = this.r.toString(16);
        var g = this.g.toString(16);
        var b = this.b.toString(16);
        if (r.length == 1) r = '0' + r;
        if (g.length == 1) g = '0' + g;
        if (b.length == 1) b = '0' + b;
        return '#' + r + g + b;
    }
}

// set our iframe text color based on the page text color
var tpe_comments_form_color = getStyle(tpe_comments_signin,"color");
var rgbcolor = new RGBColor(tpe_comments_form_color);
if( rgbcolor.ok ){
    TPConnect.text_color = rgbcolor.toHex();
}

// not as complicated as it looks, just checks two levels of signin nodes for anchor tags
var tpe_comments_anchor;
for( var i = 0; i < tpe_comments_signin.childNodes.length; i++){
    if( tpe_comments_signin.childNodes[ i ].tagName == 'A' ){
        tpe_comments_anchor = tpe_comments_signin.childNodes[ i ];
    }else{
        if( tpe_comments_signin.childNodes[ i ].childNodes.length ){
            for( var j = 0; j < tpe_comments_signin.childNodes[ i ].childNodes.length; j++ ){
                if( tpe_comments_signin.childNodes[ i ].childNodes[ j ].tagName == 'A' )
                    tpe_comments_anchor = tpe_comments_signin.childNodes[ i ].childNodes[ j ];
            }
        }
    }
}
if( tpe_comments_anchor ){
    var tpe_comments_form_acolor = getStyle(tpe_comments_anchor,"color");
    var rgbacolor = new RGBColor(tpe_comments_form_acolor);
    if( rgbacolor.ok ){
        TPConnect.text_acolor = rgbacolor.toHex();
    }   
}

TPConnect.width = tpe_comments_signin.offsetWidth;
var widthattr = '';
var moreurl = '';
if( TPConnect.text_color )
    moreurl += '&color='+ encodeURIComponent(TPConnect.text_color);
if( TPConnect.text_acolor )
    moreurl += '&acolor='+ encodeURIComponent(TPConnect.text_acolor);
if( TPConnect.width ){
    moreurl += '&width='+ encodeURIComponent(TPConnect.width - 6);
    widthattr = 'width="'+TPConnect.width+'"';
}

//<div class="comments-post" id="comments-post">
var tpe_comments_form = document.createElement('div');
tpe_comments_form.id = 'comments-post';
tpe_comments_form.innerHTML = '<iframe id="comments-post-iframe" class="comments-post-iframe" height="440" '+ widthattr +' frameborder="0" scrolling="no" allowtransparency="true" src="http://www.typepad.com/connect/blogs/6a0167607c2431970b0162ffdc445e970d/post_comment?asset_id=6a0167607c2431970b017d4099ad94970c'+ moreurl +'"></iframe>';
tpe_dataContainer.appendChild(tpe_comments_form);


//<div class="comments-action" id="comments-action">
var tpe_action_iframe = document.createElement('div');
tpe_action_iframe.id = 'comments-action';
tpe_action_iframe.innerHTML = '<iframe id="action-iframe" width="0" height="0" frameborder="0"></iframe>';
tpe_dataContainer.appendChild(tpe_action_iframe);

var tpe_comments_logo = document.createElement('div');
tpe_comments_logo.innerHTML = '<a href="http://www.typepad.com/"><img style="margin-bottom:20px;border:none;" src="http://static.typepad.com/.shared:v6bc83ba:typepad:zh_cn/images/tp-powered-badge.png" alt="powered by Typepad"/></a>';
tpe_dataContainer.appendChild(tpe_comments_logo);

///// Setup prerequisite static scripts

var tpe_scripts = document.createElement('script');
tpe_scripts.type = 'text/javascript';
tpe_scripts.src  = 'http://static.typepad.com/.shared:v6bc83ba:typepad:zh_cn/js/yui/yahoo-dom-event.js?/js/yui/connection-min.js,/js/yui/json-min.js,/js/jsonrpc-client.js,/js/client-tc.js';
tpe_dataContainer.appendChild(tpe_scripts);


var allCommentsEl = document.getElementById('all-comments');
var commentsEl = tpe_contentContainer;

///// Finally, make the first call to build the comment thread
var tpe_script = document.createElement('script');
tpe_script.type = 'text/javascript';
tpe_script.src  = TPConnect.embed_src
                   + '?asset_id=' + TPConnect.post_xid
                   + '&d=0'
                   + '&p=1'
                   + '&permalink='       + encodeURIComponent(tpe_url)
                   + '&message='         + encodeURIComponent(tpe_message)
                   + '&title='           + encodeURIComponent(tpe_title)
                   + '&show_count='      + encodeURIComponent(tpe_show_count)
                   + '&sort_order='      + encodeURIComponent(tpe_sort)
                   + '&show_threshold='  + encodeURIComponent(tpe_show_threshold)
                   + '&category_id='     + encodeURIComponent(tpe_category_id)
                   + '&developer='       + encodeURIComponent(tpe_developer)
                   + '&ifrs='            + encodeURIComponent(tpe_iframe_css)
                   + '&ts='              + tpe_date.getTime();

if ( window.location.hash ) {
    var found = window.location.hash.match( /^#comment-(6a[a-z0-9]{32})$/ );
    if ( found )
        tpe_script.src += '&posted_comment=' + found[ 1 ];
}
tpe_dataContainer.appendChild(tpe_script);

} )();
