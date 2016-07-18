<?php
/**
 * Gallery record as json
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * $_GET params
 * @param string "id_media"
 */

use hng2_base\repository\media_repository;

include "../../config.php";
include "../../includes/bootstrap.inc";

header("Content-Type: application/json; charset=utf-8");
if( ! $account->_exists ) die(json_encode(array("message" => $language->errors->page_requires_login )));

if( empty($_GET["id_media"]) ) die(json_encode(array("message" => $current_module->language->messages->missing->id )));

$repository = new media_repository();
$record = $repository->get($_GET["id_media"]);

if( is_null($record) ) die(json_encode(array("message" => $current_module->language->messages->item_not_found )));

echo json_encode(array("message" => "OK", "data" => $record->get_as_associative_array()));
