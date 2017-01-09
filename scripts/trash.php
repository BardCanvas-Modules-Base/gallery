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

use hng2_base\config;
use hng2_media\media_repository;

include "../../config.php";
include "../../includes/bootstrap.inc";

header("Content-Type: text/plain; charset=utf-8");
if( ! $account->_exists ) die($language->errors->page_requires_login);

if( empty($_GET["id_media"]) ) die($current_module->language->messages->missing->id);

$repository = new media_repository();
$item       = $repository->get($_GET["id_media"]);

if( is_null($item) ) die($current_module->language->messages->item_not_found);

if( $account->level < config::MODERATOR_USER_LEVEL && ! $account->has_admin_rights_to_module("gallery")
    && $account->id_account != $item->id_author  )
    die($current_module->language->messages->item_not_yours);

$repository->trash($_GET["id_media"]);

if( $account->id_account != $item->id_author )
    send_notification($item->id_author, "warning", replace_escaped_vars(
        $current_module->language->messages->toolbox->trashed_ok,
        '{$title}',
        convert_emojis($item->title)
    ));

echo "OK";
