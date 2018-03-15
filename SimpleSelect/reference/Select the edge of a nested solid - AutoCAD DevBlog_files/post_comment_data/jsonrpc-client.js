if( typeof TC == "undefined" ) {
    var TC = { };
}

TC.JSONRPCClient = function( uri ) {
    this.uri = uri;
}

TC.JSONRPCClient.prototype.request = function( o ) {
        var req = {
            method: o.method,
            params: [ o.params ]
        };
        var c_cb = o.callback;
        var req_cb = {
            success: function( o ) {
                var res = YAHOO.lang.JSON.parse( o.responseText );
                c_cb( res );
            },
            failure: function( o ) {
                c_cb( { error: o.responseText } );
            }
        };
        YAHOO.util.Connect.setDefaultPostHeader( 'text/javascript+json' );
        YAHOO.util.Connect.asyncRequest(
            'POST',
            this.uri,
            req_cb,
            YAHOO.lang.JSON.stringify( req )
        );
}
