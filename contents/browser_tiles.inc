<?php
/**
 * Media browser - table layout
 *
 * @var media_record[] $records
 * @var string         $order
 *
 * $_GET params
 * @param "embedded_mode"
 */

use hng2_media\media_record;

$thn_width  = current(explode("x", $settings->get("engine.thumbnail_size")));
if( empty($thn_width) ) $thn_width = 460;
?>

<? $embedded_mode_class = $_GET["embedded_mode"] == "true" ? "embedded_mode" : ""; ?>
<div id="media_browser_table" class="tiles_container clearfix <?= $embedded_mode_class ?>">
    <? foreach( $records as $row ):
        
        $trailing_thn_error = "";
        $thumbnail_to_show  = $row->get_thumbnail_url();
        
        if(
            $row->type == "image" && in_array($row->mime_type, array("image/jpeg", "image/png")) &&
            $_GET["embedded_mode"] != "true" &&
            $account->engine_prefs["gallery_browser_layout"] == "tiles" &&
            $account->engine_prefs["@gallery:expanded_tile_thumbnails"] == "true"
        ) {
            if( ! function_exists("gfuncs_getmakethumbnail") )
                include_once ROOTPATH . "/includes/guncs.php";
            
            $ratio      = $settings->get("engine.thumbnail_jpg_compression"); if( empty($ratio) ) $ratio = 90;
            $sourcefile = "{$config->datafiles_location}/uploaded_media/{$row->path}";
            $relpath    = dirname("{$config->full_root_path}/mediaserver/{$row->path}");
            $savepath   = dirname($sourcefile);
            
            try
            {
                $final_thumbnail = gfuncs_getmakethumbnail(
                    $sourcefile, $savepath, $thn_width, 0, THUMBNAILER_USE_WIDTH, false, $ratio, false
                );
                
                $thumbnail_to_show = "$relpath/$final_thumbnail";
            }
            catch(\Exception $e)
            {
                $trailing_thn_error = $e->getMessage();
            }
        }
        ?>
        
        <div class="tile record"
             <? if( $_GET["embedded_mode"] == "true" ): ?>onclick="prepare_item_for_embedded_callback(this)"<? endif; ?>
             data-record-id="<?= $row->id_media ?>" data-type="<?= $row->type ?>"
             data-width="<?= $row->get_item_width() ?>" data-height="<?= $row->get_item_height() ?>"
             data-href="<?= $row->get_item_url(false) ?>"
             data-thumbnail-url="<?= $thumbnail_to_show ?>"
             style="background-image: url('<?= $thumbnail_to_show ?>')">
            <? if( ! empty($trailing_thn_error) ) echo "
                <!-- $trailing_thn_error -->
                "; ?>
            <div class="contents">
                <div class="title">
                    <?= convert_emojis($row->title) ?>
                </div>
                <? if( $_GET["embedded_mode"] != "true" ): ?>
                    <div class="actions alignright">
                        <? if($row->status != "trashed"): ?>
                            <? if($row->type == "image"): ?>
                                <a class="action prettyPhoto" href="<?= $row->get_item_url() ?>">
                                    <span class="fa fa-search fa-2x fa-fw"></span>
                                </a>
                            <? else: ?>
                                <? $url = "{$row->get_item_embeddable_url()}?ajax=true&width={$row->get_item_width()}&height={$row->get_item_height()}"; ?>
                                <a class="action prettyPhoto" href="<?= $url ?>">
                                    <span class="fa fa-search fa-2x fa-fw"></span>
                                </a>
                            <? endif; ?>
                        <? endif; ?>
                        
                        <? if( $row->can_be_edited() ): ?>
                            <span class="action pseudo_link" onclick="edit_media('<?= $row->id_media ?>')">
                                <span class="fa fa-pencil fa-2x fa-fw"></span>
                            </span>
                        <? endif; ?>
                        <? if( $row->can_be_deleted() && $row->status != "trashed" ): ?>
                            <span class="action pseudo_link"
                                  onclick="if( confirm('<?= $language->confirm_action ?>') ) trash_media('<?= $row->id_media ?>')">
                                <span class="fa fa-trash fa-2x fa-fw"></span>
                            </span>
                        <? endif; ?>
                    </div>
                <? endif; ?>
            </div>
        </div>
        
    <? endforeach; ?>
</div>
