
function load_media_browser_in_tinymce_dialog(editor, width, height, media_type_filter, callback)
{
    var $messages = $('#gallery_strings_for_tinymce');
    var title     = $messages.find('.browser_dialog_title').text();
    
    var url = $_FULL_ROOT_PATH
        + '/gallery/index.php' 
        + '?embedded_mode=true'
        + '&search_type=' + media_type_filter
        + '&callback='    + callback
        + '&wasuuup='     + wasuuup();
    
    editor.windowManager.open({
        title:  title,
        url:    url,
        width:  width,
        height: height
    });
}

//noinspection JSUnusedGlobalSymbols,JSUnusedLocalSymbols
/**
 * This function is the callback made by the browser in "embedded mode".
 * It is called from within the browser iframe.
 *
 * @param id_media
 * @param type
 * @param file_url
 * @param thumbnail_url
 * @param width
 * @param height
 * @param embed_width
 */
function inject_selected_gallery_image_in_tinymce(
    id_media, type, file_url, thumbnail_url, width, height, embed_width
) {
    var $strings = $('#post_gallery_embed_strings');
    
    if( type != 'image' )
    {
        var message = $strings.find('.invalid_type_for_image').text();
        alert( message );
        
        return;
    }
    
    var html = '<img data-id-media="' + id_media + '" data-media-type="image" src="' + file_url + '" width="' + embed_width + '">';
    
    top.tinymce.activeEditor.insertContent(html);
    top.tinymce.activeEditor.windowManager.close();
}

//noinspection JSUnusedGlobalSymbols,JSUnusedLocalSymbols
/**
 * This function is the callback made by the browser in "embedded mode".
 * It is called from within the browser iframe.
 *
 * @param id_media
 * @param type
 * @param file_url
 * @param thumbnail_url
 * @param width
 * @param height
 * @param embed_width
 */
function inject_selected_gallery_video_in_tinymce(
    id_media, type, file_url, thumbnail_url, width, height, embed_width
) {
    var $strings = $('#post_gallery_embed_strings');
    
    if( type != 'video' )
    {
        var message = $strings.find('.invalid_type_for_video').text();
        alert( message );
        
        return;
    }
    
    var pixel_url       = $_FULL_ROOT_PATH + '/gallery/media/pixel.png';
    var placeholder_url = $_FULL_ROOT_PATH + '/gallery/media/video-play-3-xxl.png';
    
    var html  = '<img data-id-media="' + id_media + '" data-media-type="video" src="' + pixel_url + '" data-href="' + file_url + '" '
              + '     width="' + width + '" height="' + height + '" '
              + '     style="background-image: url(' + placeholder_url + '), url(' + thumbnail_url + ')">';
    
    top.tinymce.activeEditor.insertContent(html);
    top.tinymce.activeEditor.windowManager.close();
}
