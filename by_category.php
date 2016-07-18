<?php
/**
 * Frontend index of media items by category
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * $_GET params:
 * @param slug
 */

include "../config.php";
include "../includes/bootstrap.inc";

use hng2_modules\categories\categories_repository;

$categories_repository = new categories_repository();

if( empty($_GET["slug"]) ) throw_fake_404();

$category = $categories_repository->get($_GET["slug"]);
if( is_null($category) ) throw_fake_404();

$template->set("showing_archive", true);
$template->set("current_category_slug", $category->slug);
$template->page_contents_include = "by_category.inc";
$template->set_page_title(replace_escaped_vars(
    $current_module->language->pages->by_category->title, '{$category}', $category->title
));
include "{$template->abspath}/main.php";
