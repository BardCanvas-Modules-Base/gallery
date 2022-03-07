<?php
/**
 * Inject image descriptions as alt attribute on images embedded from the gallery.
 * Called from the posts/post_record_class/get_processed_content extension point.
 * 
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

use hng2_media\media_repository;

include_once ROOTPATH . "/lib/phpQuery-onefile.php";

global $config, $settings;

if( $settings->get("modules:gallery.inject_alt_from_descriptions") != "true" ) return;

$contents = $config->globals["processing_contents"];

$pq = phpQuery::newDocumentHTML($contents, "utf-8");

foreach($pq->find('img[data-media-type="image"]') as $img)
{
    $img = pq($img);
    $alt = $img->attr("alt");
    
    if( ! empty($alt) ) return;
    
    $item_id = $img->attr("data-id-media");
    if( empty($item_id) ) return;
    if( ! is_numeric($item_id) ) return;
    
    static $media_repo = null;
    if( is_null($media_repo) ) $media_repo = new media_repository();
    
    $item = $media_repo->get($item_id);
    if( is_null($item) ) return;
    
    $desc = trim(strip_tags($item->description));
    if( empty($desc) ) return;
    
    $desc = str_replace(array("\r\n", "\r", "\n"), " ", $desc);
    $desc = preg_replace("/ +/", " ", $desc);
    
    $search  = $img->htmlOuter();
    $img->attr("alt", htmlspecialchars($desc));
    $replace = $img->htmlOuter();
    
    $contents = str_replace($search, $replace, $contents);
}

$config->globals["processing_contents"] = $contents;
