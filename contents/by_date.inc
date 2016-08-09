<?php
/**
 * Frontend index of gallery items by date
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * @var config $config
 * @var template $template
 * @var string $showing_date
 * @var string $start_date
 * @var string $end_date
 */

use hng2_base\config;
use hng2_media\media_repository;
use hng2_base\template;

$media_repository      = new media_repository();
$items_data            = $media_repository->get_for_date_archive($start_date, $end_date);
$pagination_url_prefix = "$config->full_root_url/date/{$_GET["date"]}/";
$template->set("pagination_url_prefix", $pagination_url_prefix);
?>

<h1 class="clearfix">
    <?= replace_escaped_vars($current_module->language->pages->by_date->title, '{$date}', $showing_date) ?>
    <span class="pull-right"><?= $items_data->pagination["this_page_number"]; ?>/<?= $items_data->pagination["total_pages"]; ?></span>
</h1>

<? if($items_data->count == 0): ?>
    <div class='framed_content state_ko'>
        <span class='fa fa-warning'><?= $current_module->language->pages->by_date->no_records ?></span>
    </div>
    <? return; ?>
<? endif; ?>

<? include "{$template->abspath}/segments/media_index_renderer.inc"; ?>

