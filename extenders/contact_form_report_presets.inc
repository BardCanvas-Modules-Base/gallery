<?php
/**
 * Contact extender: sets subject and body when the contact form is invoked
 * via GET with the next arguments: action=report & type=media & id=xxxxxxx
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Trailing vars:
 * @var module     $this_module self (commments)
 *
 * Imported config globals:
 * • string @contact:subject
 * • string @contact:content
 * • array  @contact:show_errors
 */

use hng2_base\module;
use hng2_media\media_repository;

global $config;

if( empty($_GET["action"]) ) return;
if( empty($_GET["type"]) ) return;
if( $_GET["type"] != "media" ) return;

if( empty($_GET["id"]) )
{
    $config->globals["@contact:show_errors"][] = $this_module->language->reporting->id_not_set;
    
    return;
}

$repository = new media_repository();
$item       = $repository->get($_GET["id"]);

if( is_null($item) )
{
    $config->globals["@contact:show_errors"][] = $this_module->language->reporting->not_found;
    
    return;
}

if( in_array($item->status, array("rejected", "spam", "trashed")) )
{
    $config->globals["@contact:show_errors"][] = $this_module->language->reporting->already_processed;
    
    return;
}

if( in_array($item->status, array("reviewing")) )
{
    $config->globals["@contact:show_errors"][] = $this_module->language->reporting->in_review;
    
    return;
}

$config->globals["@contact:subject"] = replace_escaped_vars(
    $this_module->language->reporting->subject,
    array('{$item_id}', '{$item_title}', '{$item_author}'),
    array($item->id_media, $item->title, $item->author_display_name)
);

$config->globals["@contact:content"] = replace_escaped_vars(
    $this_module->language->reporting->content,
    array('{$item_author}', '{$date}', '{$description}', '{$permalink}'),
    array(
        $item->author_display_name,
        $item->creation_date . " (" . time_elapsed_string($item->creation_date). ")",
        $item->description,
        $item->get_page_url(true)
    )
);
