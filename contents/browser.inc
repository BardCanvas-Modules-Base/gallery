<?
/**
 * Gallery browser
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

use hng2_base\account;
use hng2_base\config;
use hng2_media\media_repository;
use hng2_tools\record_browser;

$record_browser_key = "{$config->website_key}_{$current_module->name}";
if( $_GET["embedded_mode"] == "true" ) $record_browser_key .= "_embedded";
$browser = new record_browser($record_browser_key);

$repository = new media_repository();

/**
 * @var int    $offset
 * @var int    $limit
 * @var int    $order
 * @var string $search_for
 * @var string $search_type
 * @var string $search_tag
 * @var string $search_status
 * @var string $search_author
 * 
 */
#region Nav filters
$built_vars = $browser->build_vars(20, 4);
foreach($built_vars as $key => $val) $$key = $val;

$where = array();

$search_for = empty($search_for) ? "" : trim($search_for);
if( ! empty($search_for) )
{
    if( strpos($search_for, ",") !== false )
    {
        $search_for_exploded = explode(",", $search_for);
        $where_line          = "";
        foreach( $search_for_exploded as $this_term )
        {
            $this_term = trim($this_term);
            if( ! empty($this_term) )
            {
                if( is_numeric($this_term) )
                {
                    $where_line .= "id_media    =     '{$this_term}'     ";
                }
                else
                {
                    $where_line .= "path        like '%{$this_term}%' or ";
                    $where_line .= "title       like '%{$this_term}%' or ";
                    $where_line .= "description like '%{$this_term}%'    ";
                }
            }
        }
        $where[] = "({$where_line})";
    }
    else
    {
        $search_for = trim($search_for);
        if( is_numeric($search_for) )
            $where[] 
                = "( id_media    =     '{$search_for}'  )"
            ;
        else
            $where[] 
                = "( path        like '%{$search_for}%' or "
                . "  title       like '%{$search_for}%' or "
                . "  description like '%{$search_for}%' )"
            ;
    }
}

if( $_GET["embedded_mode"] == "true" )
{
    $where["status"] = "published";
}
else
{
    if( $account->level < config::MODERATOR_USER_LEVEL && ! $account->has_admin_rights_to_module("gallery") )
    {
        if( $search_status == "hidden" ) $search_status = "";
        
        if( empty($search_for) )
        {
            if( empty($search_status) ) $where[] = "status not in ('trashed', 'hidden')";
            else                        $where[] = "status = '$search_status'";
        }
        else
        {
            if( empty($search_status) ) $where[] = "status not in ('hidden')";
            else                        $where[] = "status = '$search_status'";
        }
        $where["id_author"] = $account->id_account;
    }
    else
    {
        if( ! empty($search_author) ) $where["id_author"] = $_REQUEST["search_author"];
        if( ! empty($search_status) ) $where[] = "status = '$search_status'";
    }
}

# This one is an override from the embedded mode
if( ! empty($_GET["search_type"]) ) $search_type = $_GET["search_type"];

# Standard evaluation
if( ! empty($search_type) ) $where[] = "type = '$search_type'";

# By tags
if( ! empty($search_tag) ) $where[] = "(title like '%#{$search_tag}%' or description like '%#{$search_tag}%')";
#endregion

#region Nav pointers
$record_count = $repository->get_record_count($where);
$pagination   = $browser->build_pagination($record_count, $limit, $offset);
#endregion

#region Data grabbing
switch( $order )
{
    case  1: $sqlorder = "publishing_date asc";  break;
    case  2: $sqlorder = "publishing_date desc"; break;
    case  3: $sqlorder = "last_update asc";      break;
    case  4: $sqlorder = "last_update desc";     break;
    case  5: $sqlorder = "views asc";            break;
    case  6: $sqlorder = "views desc";           break;
    case  7: $sqlorder = "comments_count asc";   break;
    case  8: $sqlorder = "comments_count desc";  break;
}

$records = $repository->find($where, $limit, $offset, $sqlorder);
# echo "<pre>{$repository->get_last_query()}</pre>";
#endregion

if( $_GET["embedded_mode"] == "true" )
    $account->engine_prefs["gallery_browser_layout"] = "tiles";
else if( empty($account->engine_prefs["gallery_browser_layout"]) )
    $account->engine_prefs["gallery_browser_layout"] = "table";
?>

<? if( ! empty($_GET["edit_item"]) ): ?>
    <script type="text/javascript"> $(document).ready(function() { edit_media('<?= $_GET["edit_item"] ?>') }); </script>
<? endif; ?>

<? if( ! empty($_GET["trash_item"]) ): ?>
    <script type="text/javascript"> $(document).ready(function() { trash_media('<?= $_GET["trash_item"] ?>') }); </script>
<? endif; ?>

<h1 class="clearfix">
    <button id="refresh_media_browser" class="pull-right multirecord_actions_table_refresher"
            onclick="$('#filter_form').submit()">
        <span class="fa fa-refresh"></span>
        <?= $language->refresh ?>
    </button>
    
    <?= $current_module->language->index->title; ?>
    
    <? if( $_GET["embedded_mode"] != "true" ): ?>
        <span id="media_browser_mode_switches" data-current-mode="<?= $account->engine_prefs["gallery_browser_layout"] ?>">
            <span class="pseudo_link fa fa-th-large" onclick="switch_media_browser_mode('tiles')"
                  data-trigger-for="tiles" title="<?= $current_module->language->record_nav->modes->tiles ?>"></span>
            <span class="pseudo_link fa fa-th-list" onclick="switch_media_browser_mode('table')"
                  data-trigger-for="table" title="<?= $current_module->language->record_nav->modes->table ?>"></span>
        </span>
    <? endif; ?>
    
    <button onclick="prepare_media_addition()">
        <span class="fa fa-upload"></span>
        <?= $current_module->language->index->buttons->add ?>
    </button>
</h1>

<div class="filtering clearfix">
    <? $url = $_SERVER["PHP_SELF"] . "?wasuuup=" . mt_rand(1, 65535); ?>
    <? if( $_GET["embedded_mode"] == "true" ) $url .= "&embedded_mode=true&callback={$_GET["callback"]}"; ?>
    
    <script type="text/javascript">
        $(document).ready(function()
        {
            $('#filter_form').keypress(function(e) {
                if( (e.keyCode == 13) && (e.target.type != "textarea") )
                {
                    e.preventDefault();
                    $(this).submit();
                }
            });
        });
    </script>
    
    <form name="filter_form" id="filter_form" method="get" action="<?= $url ?>">
        
        <input type="hidden" name="mode"          value="set_filter">
        <input type="hidden" name="order"         value="<?= $order ?>">
        <input type="hidden" name="offset"        value="<?= $offset ?>">
        <input type="hidden" name="search_tag"    value="<?= $search_tag ?>">
        
        <div>
            <span style="float: right;">
                <?= $browser->get_pagination_button("previous", "paginate", $pagination) ?>
                <?= $language->record_nav->page ?>
                <?= $pagination["this_page_number"]; ?>/<?= $pagination["total_pages"]; ?>
                (<?= $pagination["total_records"]; ?> <?= $language->record_nav->entries ?>)
                <?= $browser->get_pagination_button("next", "paginate", $pagination) ?>
            </span>
            
            <?= $language->record_nav->search ?>
            <input type="text" name="search_for"
                   value="<?= htmlspecialchars($search_for) ?>" size="37"
                   placeholder="<?= $current_module->language->record_nav->search_placeholder ?>">
            
            <? $style = ( ! empty($_GET["embedded_mode"]) && ! empty($_GET["search_type"]) ) ? "display: none;" : ""; ?>
            <select name="search_type" style="<?= $style ?>" onchange="$('#filter_form').submit();">
                <option <? if($search_type == "") echo "selected"; ?> value="">
                    &lt;<?= $current_module->language->media_types->all ?>&gt;
                </option>
                <? foreach($current_module->language->media_types->media as $item): ?>
                    <option <? if($item["type"] == $search_type) echo "selected" ?> value="<?= $item["type"] ?>">
                        <?= trim($item->plural) ?>
                    </option>
                <? endforeach; ?>
            </select>
            
            <? if( $account->level >= config::MODERATOR_USER_LEVEL || $account->has_admin_rights_to_module("gallery") ): ?>
                <select name="search_author" onchange="$('#filter_form').submit();">
                    <option <? if( empty($search_author) ) echo "selected"; ?> value="">&lt;<?= $current_module->language->record_nav->filters->from_everyone ?>&gt;</option>
                    <option <? if( $search_author == $account->id_account ) echo "selected"; ?> value="<?= $account->id_account ?>"><?= $current_module->language->record_nav->filters->mine ?></option>
                    <? if( ! empty($search_author) && $search_author != $account->id_account ): $author = new account($search_author); ?>
                        <option selected value="<?= $search_author ?>"><?= replace_escaped_vars(
                                $current_module->language->record_nav->filters->from_user,
                                '{$user}',
                                $author->_exists ? $author->display_name : $search_author
                            ) ?></option>
                    <? endif; ?>
                </select>
            <? endif; ?>
            
            <span class="inline-block">
                <?= $language->record_nav->show ?>
                <select name="limit" onchange="$('#filter_form').submit();">
                    <? foreach(array(5, 10, 20, 30, 40, 50, 100, 200, 300, 400, 500) as $recs): ?>
                        <option <? if($limit == $recs) echo "selected" ?> value="<?= $recs ?>"><?= $recs ?></option>
                    <? endforeach; ?>
                </select>
                <?= $language->record_nav->recs_per_page ?>
                
                <button type="submit"><?= $language->record_nav->buttons->apply ?></button>
                <button onclick="reset_filter();"><?= $language->record_nav->buttons->reset ?></button>
            </span>
            
        </div>
    
        <? $style = ( ! empty($_GET["embedded_mode"]) ) ? "display: none;" : ""; ?>
        <div class="framed_radios" style="margin: 5px 0 0 0; <?= $style ?>">
            <label class="framed_content inlined <? if($search_status == "") echo "state_active"; ?>">
                <input type="radio" name="search_status" <? if($search_status == "") echo "checked"; ?> value=""
                       onchange="$('#filter_form').submit()">
                <?= $current_module->language->record_nav->filters->any_status ?>
            </label>
        
            <label class="framed_content inlined <? if($search_status == "draft") echo "state_active"; ?>">
                <input type="radio" name="search_status" <? if($search_status == "draft") echo "checked"; ?> value="draft"
                       onchange="$('#filter_form').submit()">
                <?= $current_module->language->status_labels->draft ?>
            </label>
        
            <label class="framed_content inlined <? if($search_status == "published") echo "state_active"; ?>">
                <input type="radio" name="search_status" <? if($search_status == "published") echo "checked"; ?> value="published"
                       onchange="$('#filter_form').submit()">
                <?= $current_module->language->status_labels->published ?>
            </label>
        
            <label class="framed_content inlined <? if($search_status == "reviewing") echo "state_active"; ?>">
                <input type="radio" name="search_status" <? if($search_status == "reviewing") echo "checked"; ?> value="reviewing"
                       onchange="$('#filter_form').submit()">
                <?= $current_module->language->status_labels->reviewing ?>
            </label>
        
            <? if( $account->level >= config::MODERATOR_USER_LEVEL || $account->has_admin_rights_to_module("gallery") ): ?>
                <label class="framed_content inlined <? if($search_status == "hidden") echo "state_active"; ?>">
                    <input type="radio" name="search_status" <? if($search_status == "hidden") echo "checked"; ?> value="hidden"
                           onchange="$('#filter_form').submit()">
                    <?= $current_module->language->status_labels->hidden ?>
                </label>
            <? endif; ?>
            
            <label class="framed_content inlined <? if($search_status == "trashed") echo "state_active"; ?>">
                <input type="radio" name="search_status" <? if($search_status == "trashed") echo "checked"; ?> value="trashed"
                       onchange="$('#filter_form').submit()">
                <?= $current_module->language->status_labels->trashed ?>
            </label>
            
            <? if($account->has_admin_rights_to_module("gallery") && $search_status == "trashed"): ?>
                <button onclick="empty_trash(); return false;" style="font-size: 12pt; padding: 5px;">
                    <span class="fa fa-trash"></span>
                    <?= $current_module->language->empty_trash->caption ?>
                </button>
            <? endif; ?>
            
        </div>
    </form>
    
    <script type="text/javascript">
        function set_filter_var(name, val, send)
        {
            if( typeof send == 'undefined' ) send = true;
        
            var $form = $('#filter_form');
            $form.find('input[name="' + name + '"]').val(val);
            if( send ) $form.submit();
        }
    </script>
    
    <?
    if( $_GET["embedded_mode"] == "true" ):
        $tags = $repository->get_grouped_tag_counts("", 3, 10);
        if( count($tags) > 0):
            ?>
            <div class="records_browser filter_links" style="margin-top: 10px;">
                <? foreach($tags as $tag => $count): ?>
                    <span class="framed_content pseudo_link <? if($search_tag == $tag) echo "state_active"; ?>"
                          onclick="set_filter_var('search_tag', '<?= $tag ?>')"><span 
                          class="fa fa-hashtag"></span><?= $tag ?> (<?= $count ?>)</span>
                <? endforeach; ?>
                <span class="framed_content pseudo_link" onclick="set_filter_var('search_tag', '')"><?= $language->none ?></span>
            </div>
            <?
        endif;
    endif; ?>
</div>

<? if( $record_count == 0 ): ?>
    <div class="framed_content state_ko">
        <span class="fa fa-info-circle"></span>
        <?= $language->record_nav->no_records_found ?>
    </div>
    <? return; ?>
<? endif;  ?>

<?
if( $_GET["embedded_mode"] != "true" && ($account->level >= config::MODERATOR_USER_LEVEL || $account->has_admin_rights_to_module("gallery")) && $search_status != "reviewing" ):
    $pending = $repository->get_record_count(array("status" => "reviewing"));
    if( $pending > 0 ):
        $message = replace_escaped_vars($current_module->language->messages->pending, '{$amount}', $pending);
        ?>
        <div class="framed_content state_ko">
            <span class="pseudo_link" onclick="var $form = $('#filter_form'); reset_filter(); $form.find('input[name=search_status][value=reviewing]').prop('checked', true); $form.submit();">
                <span class="fa fa-warning"></span>
                <?= $message ?>
            </span>
        </div>
        <?
    endif;
endif;
?>

<? include __DIR__ . "/browser_{$account->engine_prefs["gallery_browser_layout"]}.inc"; ?>

<div class="pagination">
    <? $browser->render_pagination_controls("paginate", $pagination); ?>
</div>
