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

$current_module->load_extensions("browser", "prechecks");
if( $config->globals["@gallery:abort_browser_load"] ) throw_fake_401();

$template->page_contents_include = "contents/browser.inc";
$template->set_page_title($current_module->language->index->title);
include "{$template->abspath}/embeddable.php";
