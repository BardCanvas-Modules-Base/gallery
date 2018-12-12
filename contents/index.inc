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

<link rel="stylesheet" type="text/css" href="<?= $config->full_root_path ?>/gallery/media/styles~v1.1.0-1.css">
<script type="text/javascript"          src="<?= $config->full_root_path ?>/gallery/media/functions~v1.1.0-3.js"></script>

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
        
        if( $form.find('input[name="search_status"]:first').is(':visible') )
            $form.find('input[name="search_status"][value=""]').prop('checked', true);
    }
    
    //noinspection JSUnusedGlobalSymbols
    function paginate(value)
    {
        var $form = $('#filter_form');
        $form.find('input[name="offset"]').val(value);
        $form.submit();
    }
    
    function empty_trash()
    {
        var message = $('#empty_trash_confirmation').text();
        if( ! confirm(message) ) return;
        
        var url = $_FULL_ROOT_PATH + '/gallery/scripts/toolbox.php?action=empty_trash&wasuuup=' + wasuuup();
        $.blockUI(blockUI_default_params);
        $.get(url, function(response)
        {
            if( response != 'OK' )
            {
                alert(response);
                $.unblockUI();
                return;
            }
            
            $.unblockUI();
            $('#refresh_media_browser').click();
        });
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
    
    # BCM additions
    if( ! empty($_GET["bcm_platform"]) )      $url .= "&bcm_platform={$_GET["bcm_platform"]}";
    if( ! empty($_GET["bcm_version"]) )       $url .= "&bcm_version={$_GET["bcm_version"]}";
    if( ! empty($_GET["bcm_access_token"]) )  $url .= "&bcm_access_token={$_GET["bcm_access_token"]}";
    if( ! empty($_GET["bcm_force_session"]) ) $url .= "&bcm_force_session={$_GET["bcm_force_session"]}";
    if( ! empty($_GET["bcm_output_type"]) )   $url .= "&bcm_output_type={$_GET["bcm_output_type"]}";
    if( ! empty($_GET["hide_browser"]) )      $url .= "&hide_browser={$_GET["hide_browser"]}";
    
    $expanded_thumbnails_class = $account->engine_prefs["@gallery:expanded_tile_thumbnails"] == "true" ? "expanded_thumbnails" : "";
    ?>
    
    <div id="media_browser" class="ajax_record_browser <?= $expanded_thumbnails_class ?>" data-src="<?= $url ?>">
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

<!-- Multi-record actions -->
<script type="text/javascript">
    
    function toggle_record_selections(trigger)
    {
        var $trigger = $(trigger);
        var $targets = $trigger.closest('table').find('input.record_selector');
        
        if( $trigger.is(':checked') ) $targets.prop('checked', true);
        else                          $targets.prop('checked', false);
    }
    
    var total_multirecord_action_threads     = 0;
    var processed_multirecord_action_threads = 0;
    var multirecord_action_threads_interval  = null;
    
    function launch_multirecord_actions(trigger, action)
    {
        var $trigger = $(trigger);
        var $targets = $trigger.closest('.table_wrapper').find('input.record_selector:checked');
        
        var targets_count = $targets.length;
        if( targets_count == 0 )
        {
            var message = $('#multirecord_actions_none_selected').text();
            alert(message);
            return;
        }
        
        var action_caption = $trigger.text().trim();
        
        var confirmation = $('#multirecord_actions_confirmation').text();
        confirmation = confirmation.replace(/\{\$amount\}/g,   targets_count);
        confirmation = confirmation.replace(/\{\$new_flag\}/g, action_caption);
        
        if( ! confirm(confirmation) ) return;
        
        total_multirecord_action_threads     = targets_count;
        processed_multirecord_action_threads = 0;
        $trigger.closest('.multi_record_actions').block(blockUI_smallest_params);
        
        multirecord_action_threads_interval = setInterval('check_multirecord_threads()', 500);
        
        $targets.each(function()
        {
            var $this      = $(this);
            var $container = $this.closest('tr');
            var id         = $this.val();
            
            launch_multirecord_action(action, id, $container);
        });
    }
    
    function check_multirecord_threads()
    {
        console.log( 'threads: ', processed_multirecord_action_threads, ' / ',  total_multirecord_action_threads);
        
        if( processed_multirecord_action_threads < total_multirecord_action_threads ) return;
        
        clearInterval(multirecord_action_threads_interval);
        
        var message = $('#multirecord_actions_all_done').text();
        alert(message);
        
        $('.multirecord_actions_table_refresher').click();
    }
    
    function launch_multirecord_action(action, id, $container)
    {
        var url    = $_FULL_ROOT_PATH + '/gallery/scripts/toolbox.php';
        var params = {
            action:   action,
            id_media: id,
            wasuuup:  wasuuup()
        };
        
        $container.block(blockUI_smaller_params);
        $.get(url, params, function(response)
        {
            if( response != 'OK' )
            {
                throw_notification(response, 'error');
                $container.unblock();
                processed_multirecord_action_threads++;
                
                return;
            }
            
            $container.unblock().fadeOut('fast', function()
            {
                $(this).remove();
                processed_multirecord_action_threads++;
            });
        });
    }
</script>
<div id="multirecord_actions_confirmation"  style="display: none;"><?= unindent($current_module->language->multi_actions->confirmation) ?></div>
<div id="multirecord_actions_none_selected" style="display: none;"><?= unindent($current_module->language->multi_actions->none_selected) ?></div>
<div id="multirecord_actions_all_done"      style="display: none;"><?= unindent($current_module->language->multi_actions->done) ?></div>
<div id="empty_trash_confirmation"          style="display: none;"><?= unindent($current_module->language->empty_trash->confirmation) ?></div>
