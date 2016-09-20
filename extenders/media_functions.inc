<?php
/**
 * Media functions
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

use hng2_media\media_repository;

/**
 * Converts all media tags inserted from the editor into actual embeds.
 * 
 * @param $contents
 * 
 * @return string
 */
function convert_media_tags($contents)
{
    if( ! class_exists("phpQuery") ) require_once ROOTPATH . "/lib/phpQuery-onefile.php";
    
    $document = phpQuery::newDocument($contents);
    
    # Images
    $document->find('img[data-media-type="image"]')->each("autolink_media_image");
    
    # Videos
    $document->find('img[data-media-type="video"]')->each("convert_video_tag");
    
    return $document->html();
}

function convert_video_tag($element)
{
    global $media_repository, $language;
    if( empty($media_repository) ) $media_repository = new media_repository();
    
    $element = pq($element);
    
    list($href, $id_media, $width, $height) = extract_video_tag_data($element);
    //$href     = $element->attr("data-href");
    //$id_media = $element->attr("data-id-media");
    //$width    = $element->attr("width");
    //$height   = $element->attr("height");
    
    $width    = is_numeric($width)  ? "width: {$width}px;"   : "";
    $height   = is_numeric($height) ? "height: {$height}px;" : "";
    
    $item = $media_repository->get($id_media);
    if( is_null($item) )
        $markup = "<div class='missing_media_item' style='{$width} {$height}'>{$href}<br>{$language->resource_not_found}</div>";
    elseif( $item->status != "published" )
        $markup = "<div class='missing_media_item' style='{$width} {$height}'>{$href}<br>{$language->resource_unavailable}</div>";
    else
        $markup = "<video poster='{$item->get_thumbnail_url(true)}' style='{$width} {$height}' controls>" 
            . "<source src='{$item->get_item_url(true)}'>" 
            . "</video>";
    
    $element->replaceWith($markup);
}

function autolink_media_image($element)
{
    global $media_repository, $language;
    if( empty($media_repository) ) $media_repository = new media_repository();
    
    $element = pq($element);
    
    list($href, $id_media, $width, $height) = extract_video_tag_data($element);
    //$href     = $element->attr("data-href");
    //$id_media = $element->attr("data-id-media");
    //$width    = $element->attr("width");
    //$height   = $element->attr("height");
    
    $width    = is_numeric($width)  ? "width: {$width}px;"   : "";
    $height   = is_numeric($height) ? "height: {$height}px;" : "";
    
    $item = $media_repository->get($id_media);
    if( is_null($item) )
        $markup = "<div class='missing_media_item' style='{$width} {$height}'>{$href}<br>{$language->resource_not_found}</div>";
    elseif( $item->status != "published" )
        $markup = "<div class='missing_media_item' style='{$width} {$height}'>{$href}<br>{$language->resource_unavailable}</div>";
    else
        $markup = "<a href='{$item->get_page_url()}' target='_blank'>{$element}</a>";
    
    $element->replaceWith($markup);
}

/**
 * Extracts media items from contents and returns detected IDs in an array
 *
 * @param string $type     media|video
 * @param string $contents html markup
 *
 * @return array
 */
function extract_media_items($type, $contents)
{
    if( ! class_exists("phpQuery") ) require_once ROOTPATH . "/lib/phpQuery-onefile.php";
    
    $document = phpQuery::newDocument($contents);
    $ids      = array();
    
    foreach( $document->find("img[data-media-type='$type']") as $item )
        $ids[] = pq($item)->attr("data-id-media");
    
    $ids = array_unique($ids);
    
    return $ids;
}

function extract_video_tag_data($element)
{
    $element = pq($element);
    $href     = $element->attr("data-href");
    $id_media = $element->attr("data-id-media");
    $width    = $element->attr("width");
    $height   = $element->attr("height");
    
    return array($href, $id_media, $width, $height);
}
