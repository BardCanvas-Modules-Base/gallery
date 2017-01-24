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

use hng2_base\config;
use hng2_media\media_record;
?>

<? $embedded_mode_class = $_GET["embedded_mode"] == "true" ? "embedded_mode" : ""; ?>
<div class="table_wrapper">
    
    <div class="multi_record_actions" style="font-family: arial, helvetica, sans-serif; margin-bottom: 5px;">
        <span class="fixed_font">&nbsp;┌──</span>
        <button onclick="launch_multirecord_actions(this, 'trash')">
            <?= $current_module->language->multi_actions->trash ?>
        </button>
        <? if( $account->level >= config::MODERATOR_USER_LEVEL || $account->has_admin_rights_to_module("gallery") ): ?>
            <button onclick="launch_multirecord_actions(this, 'reject')">
                <?= $current_module->language->multi_actions->reject ?>
            </button>
            <button onclick="launch_multirecord_actions(this, 'review')">
                <?= $current_module->language->multi_actions->review ?>
            </button>
            <button onclick="launch_multirecord_actions(this, 'publish')">
                <?= $current_module->language->multi_actions->publish ?>
            </button>
        <? endif; ?>
    </div>
    
    <table id="media_browser_table" class="nav_table evened <?= $embedded_mode_class ?>">
        
        <thead>
        <tr>
            <th nowrap>
                <input type="checkbox" name="record_selections_trigger" onchange="toggle_record_selections(this)">
            </th>
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
            <tr class="record <? if($row->status == "trashed") echo "deleted"; elseif($row->status == "hidden") echo "disabled"; ?>"
                <? if( $_GET["embedded_mode"] == "true" ): ?>onclick="prepare_item_for_embedded_callback(this)"<? endif; ?>
                data-record-id="<?= $row->id_media ?>" data-type="<?= $row->type ?>"
                data-width="<?= $row->get_item_width() ?>" data-height="<?= $row->get_item_height() ?>" 
                data-href="<?= $row->get_item_url(false) ?>"
                data-thumbnail-url="<?= $row->get_thumbnail_url() ?>">
                
                <td nowrap>
                    <input type="checkbox" class="record_selector" name="record_selections[]" value="<?= $row->id_media ?>">
                </td>
                
                <!-- Title and links -->
                <td width="25%">
                    <div class="pseudo_link clipboard-copy" data-clipboard-text="<?= $row->id_media ?>">
                        <code>#<?= $row->id_media ?><span class="fa fa-copy fa-fw"></span></code>
                    </div>
                    
                    <div class="principal_data">
                        <? $icon = $row->type == "image" ? "fa-picture-o" : "fa-video-camera"; ?>
                        <span class="fa <?= $icon ?>"></span>
                        <?= convert_emojis($row->title) ?>
                    </div>
                    
                    <? if( $_GET["embedded_mode"] != "true" ): ?>
                        <div class="actions">
                            <? if( in_array($row->status, array("published", "draft")) ): ?>
                                <a class="action" href="<?= $row->get_page_url() ?>" target="_blank">
                                    <span class="fa fa-link"></span>
                                    <?= $language->permalink ?>
                                </a>
                            <? endif; ?>
                            
                            <? if( $row->can_be_edited() ): ?>
                                <span class="action pseudo_link" onclick="edit_media('<?= $row->id_media ?>')">
                                    <span class="fa fa-pencil"></span>
                                    <?= $current_module->language->record_nav->actions->edit ?>
                                </span>
                            <? endif; ?>
                            
                            <? if( $modules["messaging"]->enabled && $account->id_account != $row->id_author ): ?>
                                <span class="action pseudo_link" onclick="send_pm(this, '<?= $row->author_user_name ?>', '<?= htmlspecialchars($row->author_display_name) ?>')"
                                      title="<?= replace_escaped_vars( $language->contact->pm->title, '{$website_name}', $settings->get("engine.website_name") ) ?>">
                                    <span class="fa fa-inbox"></span>
                                    <?= $language->pm_author ?>
                                </span>
                            <? endif; ?>
                            
                            <? if( ($account->level >= config::MODERATOR_USER_LEVEL || $account->has_admin_rights_to_module("gallery"))
                                   && in_array($row->status, array("reviewing")) ): ?>
                                <span class="action pseudo_link greengo" onclick="publish_media('<?= $row->id_media ?>')">
                                    <span class="fa fa-check"></span>
                                    <?= $current_module->language->record_nav->actions->approve ?>
                                </span>
                            <? endif; ?>
                            
                            <? if( ($account->level >= config::MODERATOR_USER_LEVEL || $account->has_admin_rights_to_module("gallery"))
                                   && in_array($row->status, array("published", "reviewing")) ): ?>
                                <span class="action pseudo_link important" onclick="reject_media('<?= $row->id_media ?>')">
                                    <span class="fa fa-times"></span>
                                    <?= $current_module->language->record_nav->actions->reject ?>
                                </span>
                            <? endif; ?>
                            
                            <? if( $row->can_be_deleted() && $row->status != "trashed" ): ?>
                                <span class="action pseudo_link" onclick="if( confirm('<?= $language->confirm_action ?>') ) trash_media('<?= $row->id_media ?>')">
                                    <span class="fa fa-trash"></span>
                                    <?= $current_module->language->record_nav->actions->trash ?>
                                </span>
                            <? endif; ?>
                        </div>
                        
                    <? endif; ?>
                </td>
                
                <!-- Description -->
                <td>
                    <?= empty($row->description) ? "―" : convert_emojis(make_excerpt_of($row->description, 100)) ?><br>
                    
                    <?
                    echo empty($row->dimensions) ? "<span class='framed_content inlined state_ko'><span class='fa fa-warning'></span></span>" : $row->dimensions;
                    $amount = number_format($row->size / 1000, 1);
                    $amount = rtrim($amount, "0");
                    $amount = rtrim($amount, ".");
                    echo " • $amount KB • ";
                    ?>
                    
                    <span class="framed_content inlined"><?= $current_module->language->status_labels->{$row->status} ?></span>
                    <?
                    if( $row->visibility == "private" )
                        echo "• <span class='framed_content inlined state_ko'><i class='fa fa-eye-slash'></i> {$current_module->language->visibility_options->private}</span>";
                    
                    foreach($repository->get_attached_categories($row->id_media) as $category)
                        echo "<span class='category'><span class='fa fa-folder fa-fw'></span>{$category->title}</span> ";
                    
                    if( $account->level < config::MODERATOR_USER_LEVEL )
                        foreach( $row->get_filtered_tags_list() as $tag)
                            echo "<span class='hashtag'><span class='fa fa-hashtag fa-fw'></span>{$tag}</span> ";
                    else
                        if( is_array($row->tags_list) )
                            foreach( $row->tags_list as $tag)
                                echo "<span class='hashtag'><span class='fa fa-hashtag fa-fw'></span>{$tag}</span> ";
                    
                    $principal_data_addons = array();
                    $current_module->load_extensions("records_browser", "principal_data_addons");
                    if( ! empty($principal_data_addons) ){
                        echo "<hr><div class='principal_addons' style='margin-top: 5px;'>";
                        echo implode("", $principal_data_addons);
                        echo "</div>";
                    }
                    ?>
                </td>
                
                <!-- Author -->
                <td>
                    <a class="user_display_name" data-user-level="<?= $row->author_level ?>"
                       href="<?= $config->full_root_path ?>/author/<?= $row->author_user_name ?>"
                       target="_blank">
                        <span class="fa fa-user fa-fw"></span><?= $row->get_processed_author_display_name() ?>
                    </a><br>
                    
                    <? if( ! empty($row->creation_ip) && ($account->level >= config::MODERATOR_USER_LEVEL || $account->has_admin_rights_to_module("gallery")) ): ?>
                        <span>
                            <span class="fa fa-cloud fa-fw"></span><?= $row->creation_ip ?>
                        </span><br>
                        
                        <?
                        $parts   = explode("; ", $row->creation_location);
                        $isp      = array_pop($parts);
                        $country  = array_pop($parts);
                        $city     = implode("; ", $parts);
                        if( $city != "N/A; N/A") : ?>
                            <span>
                                <span class="fa fa-map-marker fa-fw"></span><?= $city ?>
                            </span><br>
                        <? endif; ?>
                        <? if($country != "N/A"): ?>
                            <span>
                                <span class="fa fa-globe fa-fw"></span><?= $country ?>
                            </span><br>
                        <? endif; ?>
                        <? if($isp != "N/A"): ?>
                            <span>
                                <span class="fa fa-building fa-fw"></span><?= $isp ?>
                            </span><br>
                        <? endif; ?>
                    <? endif; ?>
                </td>
                
                <!-- Published -->
                <td align="center">
                    <? if( $row->publishing_date == "0000-00-00 00:00:00" ): ?>
                        &mdash;
                    <? else: ?>
                        <?= time_mini_string($row->publishing_date) ?><br>
                        (<?= time_elapsed_string($row->publishing_date) ?>)
                    <? endif; ?>
                </td>
                
                <!-- Activity -->
                <td align="center">
                    <?= time_mini_string($row->last_update) ?><br>
                    (<?= time_elapsed_string($row->last_update) ?>)
                </td>
                
                <!-- Views -->
                <td align="right">
                    <?= $row->views ?>
                </td>
                
                <!-- Thumbnail -->
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

