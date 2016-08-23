<?php
/**
 * Gallery item saver
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

use hng2_media\media_repository;

include "../../config.php";
include "../../includes/bootstrap.inc";

header("Content-Type: text/plain; charset=utf-8");
if( ! $account->_exists ) die($language->errors->page_requires_login);

if( empty($_POST["id_media"]) && empty($_FILES) )
    die($current_module->language->messages->missing->file);

$repository = new media_repository();
$old_item = empty($_POST["id_media"]) ? null : $repository->get($_POST["id_media"]);
$res = $repository->receive_and_save($_POST, $_FILES["file"], true);
if( ! is_object($res) ) die($res);
$item = $res;
$current_module->load_extensions("save_item", "after_saving");
echo "OK";
