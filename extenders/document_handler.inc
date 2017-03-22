<?php
/**
 * Document handler for media - By id, slug, etc.
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * @var template $template
 *
 * $_GET params:
 * @param handle
 */

use hng2_base\config;
use hng2_base\template;
use hng2_media\media_repository;

if( empty($_GET["handle"]) ) return;

$handle = trim(stripslashes($_GET["handle"]));
$parts  = explode("/", $handle);
if( count($parts) < 2 ) return;
if( $parts[0] != "media" ) return;

$id_media = $parts[1];

$repository = new media_repository();
$item = $repository->get($id_media);
if( is_null($item) ) return;

if( $account->level < config::MODERATOR_USER_LEVEL )
{
    if( $item->status == "hidden" ) throw_fake_404();
    if( $item->status == "trashed" ) throw_fake_404();
    if( $item->status == "draft" && $item->id_author != $account->id_account ) throw_fake_401();
    if( $item->status == "reviewing" && $item->id_author != $account->id_account ) throw_fake_401();
    if( $item->visibility == "private" && $account->id_account != $item->id_author ) throw_fake_401();
}

if( ($item->visibility == "users" || $item->main_category_visibility == "users") && ! $account->_exists )
    throw_fake_401();

if( $item->visibility == "level_based" && $account->level < $item->author_level )
    throw_fake_401();

if( $item->main_category_visibility == "level_based" && $account->level < $item->main_category_min_level )
    throw_fake_401();

if( $item->status == "reviewing" ) $item->allow_comments = 0;
if( $item->status == "published" ) $repository->increment_views($item->id_media);

$template->set_page_title($item->title);
$template->set("is_single_post", false);
$template->set("is_media_item_page", true);
$template->set("page_tag", "single_media_item");

$template->set("current_item",           $item);
$template->set("current_category_slug",  $item->main_category_slug);
$template->set("current_category_title", $item->main_category_title);
$template->set("current_category_id",    $item->main_category);

# @hack: The definition below is a hack that shouldn't be repeated unless strictly neccesary :P
$template->page_contents_include = "../../templates/{$template->name}/segments/single_media_item_renderer.inc";
include "{$template->abspath}/main.php";
exit;
