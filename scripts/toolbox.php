<?php
/**
 * Gallery toolbox(for users)
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * @var account           $account
 * @var settings          $settings
 * @var \SimpleXMLElement $language
 * 
 * $_GET params:
 * @param string "action"     publish|reject|trash|review|empty_trash
 * @param string "id_media"
 */

use hng2_base\account;
use hng2_base\config;
use hng2_base\settings;
use hng2_media\media_repository;

header("Content-Type: text/plain; charset=utf-8");
include "../../config.php";
include "../../includes/bootstrap.inc";

if( ! in_array($_GET["action"], array("publish", "reject", "trash", "review", "empty_trash")) )
    die($current_module->language->messages->toolbox->invalid_action);

if( $_GET["action"] != "empty_trash" && empty($_GET["id_media"]) )
    die($current_module->language->messages->missing->id);

$repository = new media_repository();

if($_GET["action"] == "empty_trash")
{
    if( ! $account->has_admin_rights_to_module("gallery") ) die($current_module->language->messages->toolbox->action_not_allowed);
    $repository->empty_trash();
    die("OK");
}

$item = $repository->get($_GET["id_media"]);
if( is_null($item) ) die($current_module->language->messages->item_not_found);

if( $account->level < config::MODERATOR_USER_LEVEL && ! $account->has_admin_rights_to_module("gallery")
    && $account->id_account != $item->id_author )
    die($current_module->language->messages->item_not_yours);

if($_GET["action"] == "publish")
{
    if( $account->level < config::MODERATOR_USER_LEVEL  && ! $account->has_admin_rights_to_module("gallery") )
        die($current_module->language->messages->toolbox->action_not_allowed);
    
    if( $item->status == "published" ) die("OK");
    
    $res = $repository->change_status($item->id_media, "published");
    
    if( empty($res) ) die("OK");
    
    $author = $item->get_author();
    send_notification($author->id_account, "information", replace_escaped_vars(
        $current_module->language->messages->toolbox->published_ok,
        array('{$title}', '{$link}'),
        array(convert_emojis($item->title), $item->get_page_url())
    ));
    
    die("OK");
}

if($_GET["action"] == "reject")
{
    if( $account->level < config::MODERATOR_USER_LEVEL && ! $account->has_admin_rights_to_module("gallery") )
        die($current_module->language->messages->toolbox->action_not_allowed);
    
    if( $item->status == "hidden" ) die("OK");
    
    $res = $repository->change_status($item->id_media, "hidden");
    
    if( empty($res) ) die("OK");
    
    $author = $item->get_author();
    send_notification($author->id_account, "warning", replace_escaped_vars(
        $current_module->language->messages->toolbox->rejected_ok,
        '{$title}',
        convert_emojis($item->title)
    ));
    
    die("OK");
}

if($_GET["action"] == "review")
{
    if( $account->level < config::MODERATOR_USER_LEVEL && ! $account->has_admin_rights_to_module("gallery") )
        die($current_module->language->messages->toolbox->action_not_allowed);
    
    if( $item->status == "reviewing" ) die("OK");
    
    $res = $repository->change_status($item->id_media, "reviewing");
    
    if( empty($res) ) die("OK");
    
    $author = $item->get_author();
    send_notification($author->id_account, "warning", replace_escaped_vars(
        $current_module->language->messages->toolbox->flagged_for_review_ok,
        '{$title}',
        convert_emojis($item->title)
    ));
    
    die("OK");
}

if($_GET["action"] == "trash")
{
    if( $item->status == "trashed" ) die("OK");
    
    $res = $repository->trash($_GET["id_media"]);
    
    if( $account->id_account != $item->id_author )
        send_notification($item->id_author, "warning", replace_escaped_vars(
            $current_module->language->messages->toolbox->trashed_ok,
            '{$title}',
            convert_emojis($item->title)
        ));
    
    die("OK");
}

die($language->errors->invalid_call);
