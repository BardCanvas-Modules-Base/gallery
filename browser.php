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
if( ! $account->_is_admin ) throw_fake_404();

$template->page_contents_include = "contents/browser.inc";
$template->set_page_title($current_module->language->index->title);
include "{$template->abspath}/embeddable.php";
