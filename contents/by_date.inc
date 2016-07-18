<?php
/**
 * Frontend index of gallery items by date
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * @var string $showing_date
 * @var string $start_date
 * @var string $end_date
 */

use hng2_base\repository\media_repository;
use hng2_tools\record_browser;

$media_repository      = new media_repository();
$media_browser         = new record_browser("");

$find_params = $media_repository->build_find_params_for_date_archive($start_date, $end_date);
$items_count = $media_repository->get_record_count($find_params->where);
$items       = $media_repository->find($find_params->where, $find_params->limit, $find_params->offset, $find_params->order);
$pagination  = $media_browser->build_pagination($items_count, $find_params->limit, $find_params->offset);

$pagination_url_prefix = "$config->full_root_url/date/{$_GET["date"]}/";
$template->set("pagination_url_prefix", $pagination_url_prefix);
?>

<h1 class="clearfix">
    <?= replace_escaped_vars($current_module->language->pages->by_date->title, '{$date}', $showing_date) ?>
    <span class="pull-right"><?= $pagination["this_page_number"]; ?>/<?= $pagination["total_pages"]; ?></span>
</h1>

<? if($items_count == 0)
{
    echo "
        <div class='framed_content state_ko'>
            <span class='fa fa-warning'>{$current_module->language->pages->by_date->no_records}</span>
        </div>
    ";
    
    return;
}
?>

<? include "{$template->abspath}/segments/media_index_renderer.inc"; ?>

