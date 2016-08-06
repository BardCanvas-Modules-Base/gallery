<?php
/**
 * Media browser
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * @var account  $account
 * @var template $template
 * 
 * $_GET params
 * @param embedded_mode
 * @param callback
 * @param search_type
 */

use hng2_base\account;
use hng2_base\template;

include "../config.php";
include "../includes/bootstrap.inc";
if( ! $account->_exists ) throw_fake_404();

$template->set_page_title($current_module->language->index->title);

if( $_GET["embedded_mode"] == "true" )
{
    $template->page_contents_include = "index.inc";
    include "{$template->abspath}/popup.php";
    die();
}

$template->page_contents_include = "contents/index.inc";
include "{$template->abspath}/admin.php";
