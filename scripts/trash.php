<?php
/**
 * Gallery item trashes
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * $_GET params
 * @param id_media
 */

use hng2_media\media_repository;

include "../../config.php";
include "../../includes/bootstrap.inc";

header("Content-Type: text/plain; charset=utf-8");
if( ! $account->_exists ) die($language->errors->page_requires_login);

if( empty($_GET["id_media"]) ) die($current_module->language->messages->missing->id);

$repository = new media_repository();
$count      = $repository->get_record_count(array("id_media" => $_GET["id_media"]));

if( $count == 0 ) die($current_module->language->messages->item_not_found);

$repository->trash($_GET["id_media"]);

echo "OK";
