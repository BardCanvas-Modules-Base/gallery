<?php
/**
 * Search extender: search by tag collection builder
 * Adds to the collection so it is properly displayed to the user
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Trailing globals:
 * @var module[] $modules
 * @var module   $current_module search
 * @var module   $this_module    self
 */

use hng2_base\module;
use hng2_media\media_repository;
use hng2_modules\search\search_by_tag_item;

global $config, $template;

$tag        = $template->get("current_tag");
$repository = new media_repository();
$items_data = $repository->get_for_tag($tag);

if( $items_data->count == 0 ) return;

$collection = array();

foreach($items_data->items as $item)
    $collection[] = new search_by_tag_item(array(
        "type"      => $item->type,
        "url"       => $item->get_page_url(),
        "title"     => $item->get_processed_title(false),
        "excerpt"   => "[".time_today_string($item->publishing_date)."] " . $item->get_description_excerpt(),
        "thumbnail" => $item->get_thumbnail_url(),
    ));

$key = trim($this_module->language->extenders->search_by_tag_builder->collection_heading);
$config->globals["search_by_tag_collections"][$key] = $collection;
$config->globals["search_by_tag_counts"][$key]      = $items_data->count;
