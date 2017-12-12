
tinymce.PluginManager.add('insert_gallery_video_in_tinymce', function(ed, url)
{
    var $messages = $('#gallery_strings_for_tinymce');
    var _title    = $messages.find('.browser_dialog_title').text();
    var _button   = $messages.find('.video_button').text();
    
    url = $_FULL_ROOT_PATH
        + '/gallery/index.php'
        + '?embedded_mode=true'
        + '&search_type=' + 'video'
        + '&callback='    + 'inject_selected_gallery_video_in_tinymce';
    
    if( typeof GLOBAL_AJAX_ADDED_PARAMS !== 'undefined' )
        for(var i in GLOBAL_AJAX_ADDED_PARAMS)
            url = url + '&' + i + '=' + encodeURI(GLOBAL_AJAX_ADDED_PARAMS[i]);
    
    url = url + '&wasuuup=' + wasuuup();
    
    var width  = $(window).width()  - 10;
    var height = $(window).height() - 50;
    
    ed.addCommand('insert_gallery_video_in_tinymce', function()
    {
        ed.windowManager.open({
            title:  _title,
            url:    url,
            width:  width,
            height: height
        });
    });
    
    // Register example button
    ed.addButton('insert_gallery_video_in_tinymce', {
        title: _button,
        cmd:   'insert_gallery_video_in_tinymce',
        image: $_FULL_ROOT_PATH + '/gallery/media/1f4f9.png',
        classes: 'gallery-module gallery-button gallery-video'
    });
});
