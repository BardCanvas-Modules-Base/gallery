<?php
/**
 * RSS feed generator extender
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Globals:
 * @var module   $current_module self
 * @var module   $this_module    self
 * 
 * Trailing vars (significant):
 * @var channel  $channel
 * @var template $template
 * @var string   $handle         Handle coming via GET, may be "_none_" or "something"
 * @var string   $handle_pattern The pattern as specified in the module_info.xml declaration
 * @var array    $handle_matches Matches of the handle in the pattern
 */

use hng2_base\module;
use hng2_media\media_repository;
use hng2_base\template;
use hng2_rss\channel;
use hng2_rss\enclosure;
use hng2_rss\item;

$channel->title       = $this_module->language->rss_feed_generator->title;
$channel->description = $this_module->language->rss_feed_generator->description;
$channel->link        = "{$config->full_root_url}/" . ($handle == "_none_" ? "" : "{$handle}/") . "feed";

$channel->title = replace_escaped_vars(
    $channel->title,
    array('{$website_name}'),
    array($settings->get("engine.website_name"))
);

$channel->description = replace_escaped_vars(
    $channel->description,
    array('{$website_name}'),
    array($settings->get("engine.website_name"))
);

$repository = new media_repository();
$records    = $repository->lookup(array(), 0, 0, "");

if( count($records) == 0 ) return;

foreach($records as $row)
{
    $item              = new item();
    $item->title       = html_entity_decode($row->title, null, "UTF-8");
    $item->link        = $row->get_page_url(true);
    $item->description = html_entity_decode($row->description, null, "UTF-8");
    $item->guid        = $row->get_page_url(true);
    
    $item->enclosure         = new enclosure();
    $item->enclosure->length = $row->size;
    $item->enclosure->type   = $row->mime_type;
    $item->enclosure->url    = $row->get_item_url(true);
    
    $item->author   = $row->author_user_name;
    $item->category = $row->main_category_slug;
    $item->comments = $item->link;
    $item->pubDate  = gmdate('D, d M Y H:i:s T', strtotime($row->publishing_date) );
    
    $channel->items[] = $item;
}
