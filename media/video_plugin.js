
tinymce.PluginManager.add('insert_gallery_video_in_tinymce', function(ed, url)
{
    var $messages = $('#gallery_strings_for_tinymce');
    var _title    = $messages.find('.browser_dialog_title').text();
    var _button   = $messages.find('.video_button').text();
    
    url = $_FULL_ROOT_PATH
        + '/gallery/index.php'
        + '?embedded_mode=true'
        + '&search_type=' + 'video'
        + '&callback='    + 'inject_selected_gallery_video_in_tinymce'
        + '&wasuuup='     + wasuuup();
    
    var width  = $(window).width()  - 20;
    var height = $(window).height() - 60;
    
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
