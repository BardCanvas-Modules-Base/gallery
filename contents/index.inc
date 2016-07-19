<?php
/**
 * Gallery index
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * $_GET params
 * @param tinymce_mode
 */
?>

<script type="text/javascript" src="<?= $config->full_root_path ?>/lib/jquery.exptextarea.js"></script>
<script type="text/javascript">$(document).ready(function() { $('.expandible_textarea').expandingTextArea() });</script>

<script type="text/javascript" src="<?= $config->full_root_path ?>/lib/tinymce-4.4.0/tinymce.min.js"></script>
<script type="text/javascript" src="<?= $config->full_root_path ?>/media/init_tinymce~v<?=$config->scripts_version?>.js"></script>
<script type="text/javascript"> tinymce.init(tinymce_defaults) </script>

<link rel="stylesheet" type="text/css" href="<?= $config->full_root_path ?>/gallery/media/styles~v<?=$config->scripts_version?>.css">
<script type="text/javascript"          src="<?= $config->full_root_path ?>/gallery/media/functions~v<?=$config->scripts_version?>.js"></script>

<script type="text/javascript">
    function reset_filter()
    {
        var $form = $('#filter_form');
        $form.find('input[name="search_for"]').val('');
        $form.find('select[name="limit"] option[value="20"]').prop('selected', true);
        $form.find('select[name="search_type"] option:first').prop('selected', true);
        $form.find('input[name="offset"]').val('0');
        $form.find('input[name="order"]').val('');
    }
    
    //noinspection JSUnusedGlobalSymbols
    function paginate(value)
    {
        var $form = $('#filter_form');
        $form.find('input[name="offset"]').val(value);
        $form.submit();
    }
</script>

<? if( $_GET["tinymce_mode"] == "true" ): ?>
    <script type="text/javascript">
        function inject_selected_item_in_active_editor(clicked_item)
        {
            var $strings = $('#gallery_strings_for_tinymce');
            
            var id    = $(clicked_item).attr('data-record-id');
            var src   = $(clicked_item).attr('data-href');
            var type  = $(clicked_item).attr('data-type');
            
            var default_width = parseInt($strings.find('.max_item_width').text());
            
            if( type == 'image')
            {
                var width     = parseInt($(clicked_item).attr('data-width'));
                var max_width = default_width;
                if( max_width == 0 ) max_width = 720;
                if( width > 0 && width < max_width ) max_width = width;
    
                var html  = '<img data-media-id="' + id + '" src="' + src + '" style="width: ' + max_width + 'px">';
    
                top.tinymce.activeEditor.insertContent(html);
                top.tinymce.activeEditor.windowManager.close();
                
                return;
            }
            
            var message = $strings.find('.media_type_not_supported').text();
            alert(message);
        }
    </script>
<? endif; ?>

<style type="text/css">
    @media all and (max-width: 800px)
    {
        #media_browser_table tr th:nth-child(4) { display: none; }
        #media_browser_table tr td:nth-child(4) { display: none; }
        #media_browser_table tr th:nth-child(5) { display: none; }
        #media_browser_table tr td:nth-child(5) { display: none; }
    }
    @media all and (max-width: 600px)
    {
        #media_browser_table tr th:nth-child(1) { width: 100%; }
        #media_browser_table tr td:nth-child(1) { width: 100%; }
        #media_browser_table tr th:nth-child(2) { display: none; }
        #media_browser_table tr td:nth-child(2) { display: none; }
        #media_browser_table tr th:nth-child(3) { display: none; }
        #media_browser_table tr td:nth-child(3) { display: none; }
        #media_browser_table tr th:nth-child(4) { display: none; }
        #media_browser_table tr td:nth-child(4) { display: none; }
        #media_browser_table tr th:nth-child(5) { display: none; }
        #media_browser_table tr td:nth-child(5) { display: none; }
        #media_browser_table tr th:nth-child(6) { display: none; }
        #media_browser_table tr td:nth-child(6) { display: none; }
        #media_browser_table tr th:nth-child(7) { display: none; }
        #media_browser_table tr td:nth-child(7) { display: none; }
    }
    .tinymce_mode .record { cursor: pointer; }
</style>

<div id="main_workarea">
    
    <? $url = "browser.php?wasuuup=" . md5(mt_rand(1, 65535)); ?>
    <? if( $_GET["tinymce_mode"] == "true" ) $url .= "&tinymce_mode=true"; ?>
    <div id="media_browser" class="ajax_record_browser" data-src="<?= $url ?>">
        <div class="framed_content state_highlight" style="padding: 100px 0; text-align: center;">
            <span class="fa fa-spinner fa-pulse"></span>
            <?= $language->wait ?>
        </div>
    </div>
    
</div>

<div id="form_workarea" style="display: none;">
    
    <h1 class="clearfix for_addition">
        <button class="pull-right" onclick="hide_media_form()">
            <span class="fa fa-reply"></span>
            <?= $language->return ?>
        </button>
        
        <?= $current_module->language->form->add_title; ?>
    </h1>
    
    <h1 class="clearfix for_edition" style="display: none">
        <button class="pull-right" onclick="hide_media_form()">
            <span class="fa fa-reply"></span>
            <?= $language->return ?>
        </button>
        
        <?= $current_module->language->form->edit_title; ?>
    </h1>
    
    <? include __DIR__ . "/form.inc"; ?>
</div>
