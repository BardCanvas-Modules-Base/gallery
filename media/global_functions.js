
// TODO: Move stuff here to a forging API

if( typeof $_POST_ADDON_FUNCTIONS == 'undefined' )
    var $_POST_ADDON_FUNCTIONS = {};

$_POST_ADDON_FUNCTIONS['insert_gallery_image_in_post_editor'] = function($trigger, $form)
{
    var editor_id = $form.find('textarea.tinymce').attr('id');
    var editor    = tinymce.get(editor_id);
    
    load_media_browser_in_tinymce_dialog(editor, $(window).width() - 20, $(window).height() - 60, 'image');
};

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
