<?php
/**
 * Frontend index of gallery items by author
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * @var config $config
 * @var template $template
 * @var account $author
 * 
 * $_GET params:
 * @param type
 */

use hng2_base\account;
use hng2_base\config;
use hng2_media\media_repository;
use hng2_base\template;

$media_repository      = new media_repository();
$items_data            = $media_repository->get_for_author($author->id_account, $_GET["type"]);
$pagination_url_prefix = "$config->full_root_url/author/{$author->user_name}/media/";
$template->set("pagination_url_prefix", $pagination_url_prefix);

$type = empty($_GET["type"]) ? "any" : $_GET["type"];
$heading_title = replace_escaped_vars(
    $current_module->language->pages->by_author->title,
    array('{$type}', '{$author}'),
    array($current_module->language->pages->by_author->types->{$type}, $author->get_processed_display_name())
);
$template->set("user_profile_tab", "gallery");
?>

<h1 class="clearfix">
    <?= $heading_title ?>
    <span class="pull-right"><?= $items_data->pagination["this_page_number"]; ?>/<?= $items_data->pagination["total_pages"]; ?></span>
</h1>

<? if($items_data->count == 0): ?>
    <div class='framed_content state_ko'>
        <span class='fa fa-warning'><?= $current_module->language->pages->by_author->no_records ?></span>
    </div>
    <? return; ?>
<? endif; ?>

<? include "{$template->abspath}/segments/media_index_renderer.inc"; ?>
