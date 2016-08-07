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

use hng2_base\template;
use hng2_modules\posts\posts_repository;

if( empty($_GET["handle"]) ) return;

$handle = trim(stripslashes($_GET["handle"]));
$parts  = explode("/", $handle);
if( count($parts) < 2 ) return;
if( $parts[0] != "media" ) return;

$id_or_slug = $parts[1];

die("PENDING!!!!");

/*
$repository = new posts_repository();
$post = $repository->get_by_id_or_slug($handle);

if( is_null($post) ) return;

$template->set_page_title($post->title);
$template->set("is_single_post", true);
$template->set("page_tag", "single_post");

$template->set("current_post",          $post);
$template->set("current_category_slug", $post->main_category_slug);
$template->set("current_category_id",   $post->main_category);

# @hack: The definition below is a hack that shouldn't be repeated unless strictly neccesary :P
$template->page_contents_include = "../../templates/{$template->name}/segments/single_post_renderer.inc";
include "{$template->abspath}/main.php";
exit;
*/