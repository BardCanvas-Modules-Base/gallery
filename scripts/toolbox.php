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
 * @param string "action"     publish|reject
 * @param string "id_media"
 */

use hng2_base\account;
use hng2_base\config;
use hng2_base\settings;
use hng2_media\media_repository;

header("Content-Type: text/plain; charset=utf-8");
include "../../config.php";
include "../../includes/bootstrap.inc";

if( ! in_array($_GET["action"], array("publish", "reject")) ) die($current_module->language->messages->toolbox->invalid_action);

if( empty($_GET["id_media"]) ) die($current_module->language->messages->missing->id);

$repository = new media_repository();

$item = $repository->get($_GET["id_media"]);
if( is_null($item) ) die($current_module->language->messages->item_not_found);

if($_GET["action"] == "publish")
{
    if($account->level < config::MODERATOR_USER_LEVEL)
        die($current_module->language->messages->toolbox->action_not_allowed);
    
    if( $comment->status == "published" ) die("OK");
    
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
    if($account->level < config::MODERATOR_USER_LEVEL)
        die($current_module->language->messages->toolbox->action_not_allowed);
    
    if( $comment->status == "hidden" ) die("OK");
    
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

die($language->errors->invalid_call);
