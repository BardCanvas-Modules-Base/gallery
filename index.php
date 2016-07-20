<?php
/**
 * Media browser
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *             
 * $_GET params
 * @param embedded_mode
 * @param callback
 */

include "../config.php";
include "../includes/bootstrap.inc";

$template->set_page_title($current_module->language->index->title);
if( $_GET["embedded_mode"] != "true" )
{
    $template->page_contents_include = "contents/index.inc";
    include "{$template->abspath}/admin.php";
}
else
{
    $template->page_contents_include = "index.inc";
    include "{$template->abspath}/popup.php";
}
