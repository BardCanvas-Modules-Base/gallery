
function load_media_browser_in_tinymce_dialog(editor, width, height, media_type_filter)
{
    var $messages = $('#gallery_strings_for_tinymce');
    var title     = $messages.find('.browser_dialog_title').text();
    
    var url = $_FULL_ROOT_PATH
        + '/gallery/index.php' 
        + '?tinymce_mode=true'
        + '&search_type=' + media_type_filter
        + '&wasuuup=' + parseInt(Math.random() * 1000000000000000);
    
    editor.windowManager.open({
        title: title,
        url:    url,
        width:  width,
        height: height
    });
}
