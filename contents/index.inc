<?php
/**
 * Gallery index
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * $_GET params
 * @param embedded_mode
 * @param callback
 * @param search_type
 */
?>

<script type="text/javascript" src="<?= $config->full_root_path ?>/lib/jquery.exptextarea.js"></script>
<script type="text/javascript">$(document).ready(function() { $('.expandible_textarea').expandingTextArea() });</script>

<link rel="stylesheet" type="text/css" href="<?= $config->full_root_path ?>/gallery/media/styles~v<?=$config->scripts_version?>.css">
<script type="text/javascript"          src="<?= $config->full_root_path ?>/gallery/media/functions~v<?=$config->scripts_version?>.js"></script>

<script type="text/javascript">
    function reset_filter()
    {
        var $form = $('#filter_form');
        $form.find('input[name="search_for"]').val('');
        $form.find('select[name="limit"] option[value="20"]').prop('selected', true);
        $form.find('input[name="offset"]').val('0');
        $form.find('input[name="order"]').val('');
        $form.find('input[name="search_tag"]').val('');
        
        $select = $form.find('select[name="search_author"]');
        if( $select.length > 0 ) $select.find('option:first').prop('selected', true);
        
        $form.find('select[name="search_status"] option:first').prop('selected', true);
        
        if( $form.find('select[name="search_type"]').is(':visible') )
            $form.find('select[name="search_type"] option:first').prop('selected', true);
    }
    
    //noinspection JSUnusedGlobalSymbols
    function paginate(value)
    {
        var $form = $('#filter_form');
        $form.find('input[name="offset"]').val(value);
        $form.submit();
    }
</script>

<? if( $_GET["embedded_mode"] == "true" ): ?>
    <!--suppress BadExpressionStatementJS, CommaExpressionJS -->
    <script type="text/javascript">
        function prepare_item_for_embedded_callback(clicked_item)
        {
            var $strings = $('#gallery_strings_for_tinymce');
            
            var id         = $(clicked_item).attr('data-record-id');
            var src        = $(clicked_item).attr('data-href');
            var type       = $(clicked_item).attr('data-type');
            var thumbnail  = $(clicked_item).attr('data-thumbnail-url');
            var src_width  = $(clicked_item).attr('data-width');
            var src_height = $(clicked_item).attr('data-height');
            
            var default_width = parseInt($strings.find('.max_item_width').text());
            
            var width, max_width;
            
            if( type == 'image' || type == 'video' )
            {
                width     = parseInt($(clicked_item).attr('data-width'));
                max_width = default_width;
                if( max_width == 0 ) max_width = 720;
                if( width > 0 && width < max_width ) max_width = width;
                
                <?= $_GET["callback"] ?>( id, type, src, thumbnail, src_width, src_height, max_width );
                
                return;
            }
            
            var message = $strings.find('.media_type_not_supported').text();
            alert(message);
        }
    </script>
<? endif; ?>

<div id="main_workarea">
    
    <?
    $url = "browser.php?wasuuup=" . md5(mt_rand(1, 65535));
    if( $_GET["embedded_mode"] == "true" ) $url .= "&embedded_mode=true&callback={$_GET["callback"]}";
    if( $_GET["search_type"] )             $url .= "&search_type={$_GET["search_type"]}";
    if( ! empty($_GET["edit_item"]) )      $url .= "&edit_item={$_GET["edit_item"]}";
    if( ! empty($_GET["trash_item"]) )     $url .= "&trash_item={$_GET["trash_item"]}";
    if( ! empty($_GET["author"]) )         $url .= "&search_author={$_GET["author"]}";
    ?>
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
    
    <? if( $_GET["embedded_mode"] != "true" && $modules["posts"]->enabled ): ?>
        <div id="form_initial_info_for_posts" class="for_addition framed_content state_highlight">
            <span class="fa fa-warning"></span>
            <?= $current_module->language->form_initial_info_for_posts ?>
        </div>
    <? endif; ?>
    
    <? include __DIR__ . "/form.inc"; ?>
</div>
