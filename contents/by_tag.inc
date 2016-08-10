<?php
/**
 * Frontend index of gallery items by tag
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * @var config   $config
 * @var template $template
 * @var string   $tag
 */

use hng2_base\config;
use hng2_media\media_repository;
use hng2_base\template;

$media_repository      = new media_repository();
$items_data            = $media_repository->get_for_tag($tag);
$pagination_url_prefix = "$config->full_root_url/tag/{$tag}/media/";
$template->set("pagination_url_prefix", $pagination_url_prefix);
?>

<h1 class="clearfix">
    <?= replace_escaped_vars($current_module->language->pages->by_tag->title, '{$tag}', $tag) ?>
    <span class="pull-right"><?= $items_data->pagination["this_page_number"]; ?>/<?= $items_data->pagination["total_pages"]; ?></span>
</h1>

<? if($items_data->count == 0): ?>
    <div class='framed_content state_ko'>
        <span class='fa fa-warning'><?= replace_escaped_vars(
                $current_module->language->pages->by_tag->no_records,
                '{$link}',
                "{$config->full_root_path}/tag/{$tag}/media"
            ) ?></span>
    </div>
    <? return; ?>
<? endif; ?>

<? include "{$template->abspath}/segments/media_index_renderer.inc"; ?>

