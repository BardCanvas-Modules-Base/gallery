<?php
/**
 * Media browser - table layout
 *
 * @var media_record[] $records
 * @var string         $order
 */

use hng2_base\repository\media_record;
?>

<div id="media_browser_table" class="tiles_container clearfix">
    <? foreach( $records as $row ): ?>
        
        <div class="tile record pseudo_link" data-record-id="<?= $row->id_media ?>"
             style="background-image: url('<?= $row->get_thumbnail_url() ?>')">
            <div class="contents">
                <div class="title">
                    <?= convert_emojis($row->title) ?>
                </div>
                <div class="actions alignright">
                    <a class="action lightbox" href="<?= $row->get_image_url() ?>">
                        <span class="fa fa-search fa-2x fa-fw"></span>
                    </a>
                    <span class="action pseudo_link" onclick="edit_media('<?= $row->id_media ?>')">
                        <span class="fa fa-pencil fa-2x fa-fw"></span>
                    </span>
                    <span class="action pseudo_link" onclick="trash_media('<?= $row->id_media ?>')">
                        <span class="fa fa-trash fa-2x fa-fw"></span>
                    </span>
                </div>
            </div>
        </div>
        
    <? endforeach; ?>
</div>
