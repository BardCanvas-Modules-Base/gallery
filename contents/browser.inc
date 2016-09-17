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
 * 
 */
#region Nav filters
$built_vars = $browser->build_vars(20, 4);
foreach($built_vars as $key => $val) $$key = $val;

$where = array();

if( $_GET["embedded_mode"] == "true" )
{
    $where["status"] = "published";
}
else
{
    if( $account->level < config::MODERATOR_USER_LEVEL )
        $where["id_author"] = $account->id_account;
    elseif( $account->level >= config::MODERATOR_USER_LEVEL && ! empty($_GET["author"]) )
        $where["id_author"] = $_GET["author"];
    
    if( $account->level < config::MODERATOR_USER_LEVEL && $search_status == "hidden" )
        $search_status = "";
    
    if( ! empty($search_status) ) $where["status"] = $search_status;
    
    if( $account->level < config::MODERATOR_USER_LEVEL )
        $where[] = "status <> 'hidden'";
}

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
                $where_line .= "id_media    =     '{$this_term}'  or ";
                $where_line .= "path        like '%{$this_term}%' or ";
                $where_line .= "title       like '%{$this_term}%' or ";
                $where_line .= "description like '%{$this_term}%'    ";
            }
        }
        $where[] = "({$where_line})";
    }
    else
    {
        $search_for = trim($search_for);
        $where[] 
            = "( id_media    =     '{$search_for}'  or "
            . "  path        like '%{$search_for}%' or "
            . "  title       like '%{$search_for}%' or "
            . "  description like '%{$search_for}%' )"
        ;
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
    <button id="refresh_media_browser" class="pull-right" onclick="$('#filter_form').submit()">
        <span class="fa fa-refresh"></span>
        <?= $language->refresh ?>
    </button>
    
    <?= $current_module->language->index->title; ?>
    
    <?
    if( ! empty($_GET["author"]) )
    {
        $author = new account($_GET["author"]);
        if( ! $author->_exists ) echo " ~ #{$_GET["author"]}";
        else                     echo " ~ " . convert_emojis($author->display_name);
    }
    ?>
    
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
    <form name="filter_form" id="filter_form" method="get" action="<?= $url ?>">
        
        <input type="hidden" name="mode"          value="set_filter">
        <input type="hidden" name="order"         value="<?= $order ?>">
        <input type="hidden" name="offset"        value="0">
        <input type="hidden" name="author"        value="<?= $_GET["author"] ?>">
        <input type="hidden" name="search_tag"    value="<?= $search_tag ?>">
        <input type="hidden" name="search_status" value="<?= $search_status ?>">
        
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
                   value="<?= htmlspecialchars($search_for) ?>" size="46"
                   placeholder="<?= $current_module->language->record_nav->search_placeholder ?>">
            
            <? $style = ( ! empty($_GET["embedded_mode"]) && ! empty($_GET["search_type"]) ) ? "display: none;" : ""; ?>
            <select name="search_type" style="<?= $style ?>">
                <option <? if($search_type == "") echo "selected"; ?> value="">
                    &lt;<?= $current_module->language->media_types->all ?>&gt;
                </option>
                <? foreach($current_module->language->media_types->media as $item): ?>
                    <option <? if($item["type"] == $search_type) echo "selected" ?> value="<?= $item["type"] ?>">
                        <?= trim($item->plural) ?>
                    </option>
                <? endforeach; ?>
            </select>
            
            <?= $language->record_nav->show ?>
            <select name="limit">
                <? foreach(array(5, 10, 20, 30, 40, 50, 100, 200, 300, 400, 500) as $recs): ?>
                    <option <? if($limit == $recs) echo "selected" ?> value="<?= $recs ?>"><?= $recs ?></option>
                <? endforeach; ?>
            </select>
            <?= $language->record_nav->recs_per_page ?>
            
            <button type="submit"><?= $language->record_nav->buttons->apply ?></button>
            <button type="submit" onclick="reset_filter();"><?= $language->record_nav->buttons->reset ?></button>
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
    else:
        ?>
        <div class="records_browser filter_links" style="margin-top: 10px;">
            <? if($account->level >= config::MODERATOR_USER_LEVEL): ?>
                <span class="framed_content <? if($_GET["author"] == $account->id_account) echo "state_active" ?> pseudo_link"
                      onclick="set_filter_var('search_status', '', false); set_filter_var('author', '<?= $account->id_account ?>')">
                    <?= $language->mine_sm ?>
                    (<?= number_format($repository->get_record_count(array("id_author" => $account->id_account))) ?>)
                </span>
                <? unset( $where["id_author"] ); ?>
            <? endif; ?>
            
            <? $where_copy = $where; $where_copy["status"] = "published"; ?>
            <span class="framed_content <? if($search_status == "published") echo "state_active" ?> pseudo_link"
                  onclick="set_filter_var('author', '', false); set_filter_var('search_status', 'published')">
                <?= $current_module->language->status_labels->published ?>
                (<?= number_format($repository->get_record_count($where_copy)) ?>)
            </span>
            
            <? $where_copy = $where; $where_copy["status"] = "reviewing";
               $reviewing = $repository->get_record_count($where_copy); ?>
            <span class="framed_content <? if($search_status == "reviewing") echo "state_active" ?> <? if($reviewing) echo "state_ko"; ?> pseudo_link"
                  onclick="set_filter_var('author', '', false); set_filter_var('search_status', 'reviewing')">
                <?= $current_module->language->status_labels->reviewing ?>
                (<?= number_format($reviewing) ?>)
            </span>
            
            <? $where_copy = $where; $where_copy["status"] = "trashed"; ?>
            <span class="framed_content <? if($search_status == "trashed") echo "state_active" ?> pseudo_link"
                  onclick="set_filter_var('author', '', false); set_filter_var('search_status', 'trashed')">
                <?= $current_module->language->status_labels->trashed ?>
                (<?= number_format($repository->get_record_count($where_copy)) ?>)
            </span>
            
            <? if($account->level >= config::MODERATOR_USER_LEVEL):
                $where_copy = $where; $where_copy["status"] = "hidden"; ?>
                <span class="framed_content <? if($search_status == "hidden") echo "state_active" ?> pseudo_link"
                      onclick="set_filter_var('author', '', false); set_filter_var('search_status', 'hidden')">
                    <?= $current_module->language->status_labels->hidden ?>
                    (<?= number_format($repository->get_record_count($where_copy)) ?>)
                </span>
            <? endif; ?>
            
            <span class="framed_content pseudo_link" onclick="set_filter_var('author', '', false); set_filter_var('search_status', '')">
                <?= $language->all_m ?>
            </span>
            
        </div>
        <?
    endif; ?>
</div>

<? if( $record_count == 0 ): ?>
    <div class="framed_content state_ko">
        <span class="fa fa-info-circle"></span>
        <?= $language->record_nav->no_records_found ?>
    </div>
    <? return; ?>
<? endif;  ?>

<? include __DIR__ . "/browser_{$account->engine_prefs["gallery_browser_layout"]}.inc"; ?>

<div class="pagination">
    <? $browser->render_pagination_controls("paginate", $pagination); ?>
</div>
