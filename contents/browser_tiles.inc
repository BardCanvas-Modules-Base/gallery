<?php
/**
 * Media browser - table layout
 *
 * @var media_record[] $records
 * @var string         $order
 *
 * $_GET params
 * @param embedded_mode
 */

use hng2_media\media_record;
?>

<? $embedded_mode_class = $_GET["embedded_mode"] == "true" ? "embedded_mode" : ""; ?>
<div id="media_browser_table" class="tiles_container clearfix <?= $embedded_mode_class ?>">
    <? foreach( $records as $row ): ?>
        
        <div class="tile record"
             <? if( $_GET["embedded_mode"] == "true" ): ?>onclick="prepare_item_for_embedded_callback(this)"<? endif; ?>
             data-record-id="<?= $row->id_media ?>" data-type="<?= $row->type ?>"
             data-width="<?= $row->get_item_width() ?>" data-height="<?= $row->get_item_height() ?>"
             data-href="<?= $row->get_item_url(false) ?>"
             data-thumbnail-url="<?= $row->get_thumbnail_url() ?>"
             style="background-image: url('<?= $row->get_thumbnail_url() ?>')">
            <div class="contents">
                <div class="title">
                    <?= convert_emojis($row->title) ?>
                </div>
                <? if( $_GET["embedded_mode"] != "true" ): ?>
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
                        <? if( $row->can_be_edited() ): ?>
                            <span class="action pseudo_link" onclick="edit_media('<?= $row->id_media ?>')">
                                <span class="fa fa-pencil fa-2x fa-fw"></span>
                            </span>
                        <? endif; ?>
                        <? if( $row->status != "trashed" ): ?>
                            <span class="action pseudo_link" onclick="trash_media('<?= $row->id_media ?>')">
                                <span class="fa fa-trash fa-2x fa-fw"></span>
                            </span>
                        <? endif; ?>
                    </div>
                <? endif; ?>
            </div>
        </div>
        
    <? endforeach; ?>
</div>
