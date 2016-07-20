<?php
/**
 * Gallery item saver
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

use hng2_base\repository\media_record;
use hng2_base\repository\media_repository;
use hng2_media\abstract_item_manager;

include "../../config.php";
include "../../includes/bootstrap.inc";

header("Content-Type: text/plain; charset=utf-8");
if( ! $account->_exists ) die($language->errors->page_requires_login);

if( empty($_POST["id_media"]) && empty($_FILES) )
    die($current_module->language->messages->missing->file);

/** @var abstract_item_manager $media_manager */
$media_manager = null;
$media_path    = "";
$thumbnail     = "";
$media_type    = "";
$mime_type     = "";
$dimensions    = "";
$size          = 0;
if( empty($_POST["id_media"]) && ! empty($_FILES["file"]) )
{
    if( ! is_uploaded_file($_FILES["file"]["tmp_name"]) )
        die($current_module->language->messages->invalid->upload);
    
    $extension = strtolower(end(explode(".", $_FILES["file"]["name"])));
    if( empty($extension) ) die($current_module->language->messages->invalid->file);
    
    if( ! isset($config->upload_file_types[$extension]) )
        die($current_module->language->messages->invalid->file);
    
    $media_manager_class = $config->upload_file_types[$extension];
    if( $media_manager_class == "system" )
        $media_manager_class = "hng2_media\\item_manager_{$extension}";
    
    try
    {
        $media_manager = new $media_manager_class(
            $_FILES["file"]["name"],
            $_FILES["file"]["type"],
            $_FILES["file"]["tmp_name"]
        );
        
        $parts         = explode(".", $_FILES["file"]["name"]); array_pop($parts);
        $file_name     = sanitize_file_name(implode(".", $parts));
        $date          = date("dHis");
        $new_file_name = "{$account->user_name}-{$date}-{$file_name}.{$extension}";
        
        $media_manager->move_to_repository($new_file_name);
        $media_path = $media_manager->get_relative_path();
        $thumbnail  = $media_manager->get_thumbnail();
        $media_type = $media_manager->get_type();
        $mime_type  = $_FILES["file"]["type"];
        $dimensions = $media_manager->get_dimensions();
        $size       = $media_manager->get_size();
    }
    catch(\Exception $e)
    {
        die(unindent(replace_escaped_vars(
            $current_module->language->messages->media_manager_exception,
            array('{$class}', '{$exception}'),
            array($media_manager_class, $e->getMessage())
        )));
    }
}

$repository = new media_repository();

$old_item = empty($_POST["id_media"]) ? null : $repository->get($_POST["id_media"]);

$item = new media_record();
$item->set_from_post();

if( empty($item->id_media) )
{
    $item->set_new_id();
    $item->type              = $media_type;
    $item->mime_type         = $mime_type;
    $item->dimensions        = $dimensions;
    $item->size              = $size;
    $item->path              = $media_path;
    $item->thumbnail         = $thumbnail;
    $item->id_author         = $account->id_account;
    $item->creation_date     = date("Y-m-d H:i:s");
    $item->creation_ip       = get_remote_address();
    $item->creation_host     = gethostbyaddr($item->creation_ip);
    $item->creation_location = forge_geoip_location($item->creation_ip);
    $item->last_update       = date("Y-m-d H:i:s");
    
    if( empty($item->title) )
        $item->title = stripslashes($account->display_name . " - " . $_FILES["file"]["name"]);
    
    if( empty($item->main_category) )
        $item->main_category = "0000000000000";
}

if( $item->main_category != $old_item->main_category )
    $repository->unset_category($old_item->main_category, $item->id_media);
$repository->set_category($_POST["main_category"], $item->id_media);

if( $item->status == "published" && $old_item->status != $item->status )
    $item->publishing_date = date("Y-m-d H:i:s");

$repository->save($item);

echo "OK";
