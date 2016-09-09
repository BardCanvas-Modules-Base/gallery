<?php
/**
 * Settings field type extenders: preinits
 * Called from within a function.
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Trailing globals:
 * @var module[] $modules
 * @var module   $current_module settings
 * @var module   $this_module    self
 */

use hng2_base\module;
?>

<style type="text/css">
    .settings_image_selector { border: 1px solid; width: 100%; }
</style>

<div id="image_selector_dialog" title="<?= $this_module->language->tinymce_strings->select_image ?>"
     style="display: none; overflow: hidden;">
    <iframe id="image_selector_target" style="width: 100%; height: 100%;" frameborder="0"
            allowtransparency="true"></iframe>
</div>

<script type="text/javascript">
    var selected_image_trigger = null;
    var return_image_size      = 'full';
    var image_dialog_title     = '<?= $this_module->language->tinymce_strings->select_image ?>';
    var invalid_type_for_image = '';
    
    function set_image(trigger, select_image_size)
    {
        selected_image_trigger = trigger;
        
        if( typeof select_image_size != 'undefined' ) return_image_size = select_image_size;
        
        var url = $_FULL_ROOT_PATH
            + '/gallery/index.php'
            + '?embedded_mode=true'
            + '&search_type=' + 'image'
            + '&callback='    + 'top.' + 'set_selected_gallery_image'
            + '&wasuuup='     + parseInt(Math.random() * 1000000000000000);
        
        $('#image_selector_target').attr('src', url);
        $('#image_selector_dialog').dialog('open');
    }
    
    function set_selected_gallery_image(
        id_media, type, file_url, thumbnail_url, width, height, embed_width
    ) {
        var $strings = $('#post_gallery_embed_strings');
        
        if( type != 'image' )
        {
            alert( invalid_type_for_image );
            
            return;
        }
    
        $('#image_selector_dialog').dialog('close');
        
        var image_to_return = return_image_size == 'full' ? file_url : thumbnail_url;
        
        var $container = $(selected_image_trigger).closest('.image_selector_container');
        $container.find('input.src_image_value').val(image_to_return);
        $container.find('.settings_image_selector').attr('src', image_to_return);
        $container.find('.settings_image_selector').closest('a').attr('href', image_to_return);
    }
    
    function remove_image(trigger)
    {
        var $container = $(trigger).closest('.image_selector_container');
        var empty_src = $container.find('.settings_image_selector').attr('data-empty-src');
        $container.find('input.src_image_value').val('');
        $container.find('.settings_image_selector').attr('src', empty_src);
    }
    
    $(document).ready(function()
    {
        $('#image_selector_dialog').dialog({
            modal:    true,
            autoOpen: false,
            width:    $(window).width() - 20,
            height:   $(window).height() - 10
        });
    })
</script>
