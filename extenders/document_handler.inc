<?php
/**
 * Document handler for media - By id, slug, etc.
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * @var template $template
 *
 * $_GET params:
 * @param handle
 */

use hng2_base\template;
use hng2_modules\posts\posts_repository;

if( empty($_GET["handle"]) ) return;

$handle = trim(stripslashes($_GET["handle"]));
$parts  = explode("/", $handle);
if( count($parts) < 2 ) return;
if( $parts[0] != "media" ) return;

$id_or_slug = $parts[1];

die("PENDING!!!!");
