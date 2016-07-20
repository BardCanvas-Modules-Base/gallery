<?
/**
 * Gallery browser
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * $_GET params
 * @param tinymce_mode
 */

use hng2_base\config;
use hng2_base\repository\media_repository;
use hng2_tools\record_browser;

$browser    = new record_browser("{$config->website_key}_{$current_module->name}");
$repository = new media_repository();

/**
 * @var int    $offset
 * @var int    $limit
 * @var int    $order
 * @var string $search_state
 * @var string $search_for
 * @var string $search_type
 * 
 */
#region Nav filters
$built_vars = $browser->build_vars(20, 4);
foreach($built_vars as $key => $val) $$key = $val;

$where = array("status <> 'trashed'");
if( $account->level < config::MODERATOR_USER_LEVEL ) $where[] = "id_author = '{$account->id_account}'";

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
        $where[] = $where_line;
    }
    else
    {
        $search_for = trim($search_for);
        $where[] 
            = "id_media    =     '{$search_for}'  or "
            . "path        like '%{$search_for}%' or "
            . "title       like '%{$search_for}%' or "
            . "description like '%{$search_for}%'"
        ;
    }
}

if( ! empty($search_type) ) $where[] = "type = '$search_type'";
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
#endregion

if( empty($account->engine_prefs["gallery_browser_layout"]) )
    $account->engine_prefs["gallery_browser_layout"] = "table";
?>

<h1 class="clearfix">
    <button id="refresh_media_browser" class="pull-right" onclick="$('#filter_form').submit()">
        <span class="fa fa-refresh"></span>
        <?= $language->refresh ?>
    </button>
    
    <?= $current_module->language->index->title; ?>
    
    <span id="media_browser_mode_switches" data-current-mode="<?= $account->engine_prefs["gallery_browser_layout"] ?>">
        <span class="pseudo_link fa fa-th-large" onclick="switch_media_browser_mode('tiles')"
              data-trigger-for="tiles" title="<?= $current_module->language->record_nav->modes->tiles ?>"></span>
        <span class="pseudo_link fa fa-th-list" onclick="switch_media_browser_mode('table')"
              data-trigger-for="table" title="<?= $current_module->language->record_nav->modes->table ?>"></span>
    </span>
    
    <button onclick="prepare_media_addition()">
        <span class="fa fa-upload"></span>
        <?= $current_module->language->index->buttons->add ?>
    </button>
</h1>

<div class="filtering clearfix">
    <? $url = $_SERVER["PHP_SELF"] . "?wasuuup=" . mt_rand(1, 65535); ?>
    <? if( $_GET["tinymce_mode"] == "true" ) $url .= "&tinymce_mode=true"; ?>
    <form name="filter_form" id="filter_form" method="get" action="<?= $url ?>">
        
        <input type="hidden" name="mode"   value="set_filter">
        <input type="hidden" name="order"  value="<?= $order ?>">
        <input type="hidden" name="offset" value="0">
        
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
        
        <select name="search_type">
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
    </form>
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
