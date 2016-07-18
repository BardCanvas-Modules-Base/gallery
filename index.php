<?php
/**
 * Media browser
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

include "../config.php";
include "../includes/bootstrap.inc";

$template->page_contents_include = "contents/index.inc";
$template->set_page_title($current_module->language->index->title);
include "{$template->abspath}/admin.php";
