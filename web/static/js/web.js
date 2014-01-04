$(document).ready(function() {
    var t = $("#editor");
    var editor = CodeMirror.fromTextArea(
	document.getElementById("editor"), 
	{
            lineNumbers: true,
            mode: "text/x-csrc",
            vimMode: false,
            showCursorWhenSelecting: true,
	    extraKeys: {
		"F11": function(cm) {
		    cm.setOption("fullScreen", !cm.getOption("fullScreen"));
		},
	    }
	});
});
