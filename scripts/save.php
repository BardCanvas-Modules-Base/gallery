<?php
/**
 * Gallery item saver
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

use hng2_base\config;
use hng2_media\media_repository;

include "../../config.php";
include "../../includes/bootstrap.inc";

header("Content-Type: text/plain; charset=utf-8");
if( ! $account->_exists ) die($language->errors->page_requires_login);

if( empty($_POST["id_media"]) && empty($_FILES) )
    die($current_module->language->messages->missing->file);

if( preg_match('/http|https|www\./i', stripslashes($_POST["title"])) )
    die($current_module->language->messages->no_urls_in_title);

$current_module->load_extensions("save_item", "initial_validations");

$repository = new media_repository();
$old_item = empty($_POST["id_media"]) ? null : $repository->get($_POST["id_media"]);

if( ! is_null($old_item) )
    if( $account->level < config::MODERATOR_USER_LEVEL && $account->id_account != $old_item->id_author )
        die($current_module->language->messages->item_not_yours);

$res = $repository->receive_and_save($_POST, $_FILES["file"], true);
if( ! is_object($res) ) die($res);
$item = $res;
$current_module->load_extensions("save_item", "after_saving");
echo "OK";
