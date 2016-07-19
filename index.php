<?php
/**
 * Media browser
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *             
 * $_GET params
 * @param tinymce_mode
 */

include "../config.php";
include "../includes/bootstrap.inc";

$template->set_page_title($current_module->language->index->title);
if( $_GET["tinymce_mode"] != "true" )
{
    $template->page_contents_include = "contents/index.inc";
    include "{$template->abspath}/admin.php";
}
else
{
    $template->page_contents_include = "index.inc";
    include "{$template->abspath}/popup.php";
}
