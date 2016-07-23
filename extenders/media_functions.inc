<?php
/**
 * Media functions
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */
use hng2_base\repository\media_repository;


/**
 * Converts all media tags inserted from the editor into actual embeds.
 * 
 * @param $contents
 * 
 * @return string
 */
function convert_media_tags($contents)
{
    if( ! class_exists("phpQuery") ) require_once ABSPATH . "/lib/phpQuery-onefile.php";
    
    $document = phpQuery::newDocument($contents);
    
    # Videos
    $document->find('img[data-media-type="video"]')->each("convert_video_tag");
    
    return $document->html();
}

function convert_video_tag($element)
{
    global $media_repository, $language;
    if( empty($media_repository) ) $media_repository = new media_repository();
    
    $element = pq($element);
    $href     = $element->attr("data-href");
    $id_media = $element->attr("data-id-media");
    $width    = $element->attr("width");
    $height   = $element->attr("height");
    
    $width    = is_numeric($width)  ? "width: {$width}px;"   : "";
    $height   = is_numeric($height) ? "height: {$height}px;" : "";
    
    $item = $media_repository->get($id_media);
    if( is_null($item) || $item->status != "published" )
        $markup = "<div class='missing_media_item' style='{$width} {$height}'>{$href}<br>{$language->file_not_found}</div>";
    else
        $markup = "<video poster='{$item->get_thumbnail_url(true)}' style='{$width} {$height}' controls>" 
            . "<source src='{$item->get_item_url(true)}'>" 
            . "</video>";
    
    $element->replaceWith($markup);
}
