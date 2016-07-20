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

use hng2_base\repository\media_record;
?>

<? $embedded_mode_class = $_GET["embedded_mode"] == "true" ? "embedded_mode" : ""; ?>
<div class="table_wrapper">
    <table id="media_browser_table" class="nav_table evened <?= $embedded_mode_class ?>">
        
        <thead>
        <tr>
            <? foreach( $browser->build_table_header($current_module->language->record_nav->columns) as $this_cell ): ?>
                <th align="<?= $this_cell->xalign ?>" <? if( $this_cell->xnowrap == true ) echo "nowrap "; ?>
                    <? if( ! empty($this_cell->xwidth) ) echo "width='$this_cell->xwidth' "; ?> class="<?= $this_cell->xclass ?>"><?
                    if( $this_cell->sort_asc->enabled == true ) echo "<img
                    src='{$config->full_root_path}/media/icons/order_asc" . ($order == $this_cell->sort_asc->order ? "_this" : "") . ".gif' 
                    align='absmiddle' width='7' height='7'  style='cursor: pointer;'
                    alt='{$this_cell->sort_asc->alt}' title='{$this_cell->sort_asc->alt}'
                    onclick=\"document.filter_form.order.value='{$this_cell->sort_asc->order}'; $('#filter_form').submit();\">&nbsp;";
                    ?><?= $this_cell->content ?><?
                    if( $this_cell->sort_desc->enabled == true ) echo "&nbsp;<img
                    src='{$config->full_root_path}/media/icons/order_desc" . ($order == $this_cell->sort_desc->order ? "_this" : "") . ".gif' 
                    align='absmiddle' width='7' height='7'  style='cursor: pointer;'
                    alt='{$this_cell->sort_desc->alt}' title='{$this_cell->sort_desc->alt}'
                    onclick=\"document.filter_form.order.value='{$this_cell->sort_desc->order}'; $('#filter_form').submit();\">";
                    ?></th>
            <? endforeach; ?>
        </tr>
        </thead>
        
        <tbody>
        <? foreach( $records as $row ): ?>
            <tr class="record"
                <? if( $_GET["embedded_mode"] == "true" ): ?>onclick="prepare_item_for_embedded_callback(this)"<? endif; ?>
                data-record-id="<?= $row->id_media ?>" data-type="<?= $row->type ?>"
                data-width="<?= $row->get_item_width() ?>" data-height="<?= $row->get_item_height() ?>" 
                data-href="<?= $row->get_item_url(false) ?>"
                data-thumbnail-url="<?= $row->get_thumbnail_url() ?>">
                <td width="25%">
                    <div class="principal_data"><?= convert_emojis($row->title) ?></div>
                    
                    <? if( $_GET["embedded_mode"] != "true" ): ?>
                        <div class="actions">
                            <span class="action pseudo_link" onclick="edit_media('<?= $row->id_media ?>')">
                                <span class="fa fa-pencil"></span>
                                <?= $current_module->language->record_nav->actions->edit ?>
                            </span>
                            <span class="action pseudo_link" onclick="trash_media('<?= $row->id_media ?>')">
                                <span class="fa fa-trash"></span>
                                <?= $current_module->language->record_nav->actions->trash ?>
                            </span>
                        </div>
                    <? endif; ?>
                </td>
                <td>
                    <?= empty($row->description) ? "―" : convert_emojis(make_excerpt_of($row->description, 100)) ?><br>
                    
                    <?
                    echo $row->dimensions;
                    $amount = number_format($row->size / 1000, 1);
                    $amount = rtrim($amount, "0");
                    $amount = rtrim($amount, ".");
                    echo " • $amount KB";
                    echo "<br>";
                    ?>
                    
                    <span class="framed_content inlined"><?= $current_module->language->status_labels->{$row->status} ?></span>
                    <?
                    foreach($repository->get_attached_categories($row->id_media) as $category)
                        echo "<span class='category'><span class='fa fa-folder fa-fw'></span>{$category->title}</span> ";
                    
                    if( ! empty($row->tags_list) )
                        foreach($row->tags_list as $tag)
                            echo "<span class='hashtag'><span class='fa fa-hashtag fa-fw'></span>{$tag}</span> ";
                    ?>
                </td>
                <td>
                    <?= convert_emojis($row->author_display_name) ?>
                </td>
                <td>
                    <?= time_elapsed_string($row->publishing_date) ?>
                </td>
                <td>
                    <?= time_elapsed_string($row->last_update) ?>
                </td>
                <td align="right">
                    <?= $row->views ?>
                </td>
                <td align="right">
                    <?= $row->comments_count ?>
                </td>
                <td>
                    <? if( $_GET["embedded_mode"] != "true" ): ?>
                        <? if($row->type == "image"): ?>
                            <a class="thumbnail prettyPhoto" href="<?= $row->get_item_url(false) ?>">
                                <img src="<?= $row->get_thumbnail_url(false) ?>">
                            </a>
                        <? else: ?>
                            <? $url = "{$row->get_item_embeddable_url()}?ajax=true&width={$row->get_item_width()}&height={$row->get_item_height()}"; ?>
                            <a class="thumbnail prettyPhoto" href="<?= $url ?>">
                                <img src="<?= $row->get_thumbnail_url(false) ?>">
                            </a>
                        <? endif; ?>
                    <? else: ?>
                        <span class="thumbnail">
                            <img src="<?= $row->get_thumbnail_url(false) ?>">
                        </span>
                    <? endif; ?>
                </td>
            </tr>
        <? endforeach; ?>
        </tbody>
    </table>
</div>

