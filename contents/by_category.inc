<?php
/**
 * Frontend index of gallery items by category
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * @var config $config
 * @var template $template
 * @var category_record $category
 */

use hng2_base\config;
use hng2_media\media_repository;
use hng2_base\template;
use hng2_modules\categories\category_record;

$media_repository      = new media_repository();
$items_data            = $media_repository->get_for_category($category->id_category);
$pagination_url_prefix = "$config->full_root_url/category/{$category->slug}/";
$template->set("pagination_url_prefix", $pagination_url_prefix);
?>

<h1 class="clearfix">
    <?= replace_escaped_vars($current_module->language->pages->by_category->title, '{$category}', $category->title) ?>
    <span class="pull-right"><?= $items_data->pagination["this_page_number"]; ?>/<?= $items_data->pagination["total_pages"]; ?></span>
</h1>

<? if($items_data->count == 0): ?>
    <div class='framed_content state_ko'>
        <span class='fa fa-warning'><?= $current_module->language->pages->by_category->no_records ?></span>
    </div>
    <? return; ?>
<? endif; ?>

<? include "{$template->abspath}/segments/media_index_renderer.inc"; ?>

