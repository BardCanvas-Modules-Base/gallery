<?php
/**
 * Media browser - table layout
 *
 * @var media_record[] $records
 * @var string         $order
 *
 * $_GET params
 * @param tinymce_mode
 */

use hng2_base\repository\media_record;
?>

<? $tinymce_mode_class = $_GET["tinymce_mode"] == "true" ? "tinymce_mode" : ""; ?>
<div id="media_browser_table" class="tiles_container clearfix <?= $tinymce_mode_class ?>">
    <? foreach( $records as $row ): ?>
        
        <div class="tile record pseudo_link"
             <? if( $_GET["tinymce_mode"] == "true" ): ?>onclick="inject_selected_item_in_active_editor(this)"<? endif; ?>
             data-record-id="<?= $row->id_media ?>" data-type="<?= $row->type ?>"
             data-width="<?= $row->get_item_width() ?>" data-href="<?= $row->get_item_url(false) ?>"
             style="background-image: url('<?= $row->get_thumbnail_url() ?>')">
            <div class="contents">
                <div class="title">
                    <?= convert_emojis($row->title) ?>
                </div>
                <? if( $_GET["tinymce_mode"] != "true" ): ?>
                    <div class="actions alignright">
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
                        <span class="action pseudo_link" onclick="edit_media('<?= $row->id_media ?>')">
                            <span class="fa fa-pencil fa-2x fa-fw"></span>
                        </span>
                        <span class="action pseudo_link" onclick="trash_media('<?= $row->id_media ?>')">
                            <span class="fa fa-trash fa-2x fa-fw"></span>
                        </span>
                    </div>
                <? endif; ?>
            </div>
        </div>
        
    <? endforeach; ?>
</div>
