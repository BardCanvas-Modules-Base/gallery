<?php
namespace hng2_modules\gallery;

use hng2_base\config;
use hng2_media\media_record;
use hng2_media\media_repository;

class toolbox
{
    public function notify_mods_on_media_submission(media_record $item)
    {
        /**
         * @var media_repository $repository
         */
        global $config, $settings, $repository, $account, $current_module;
        
        $mem_ttl = 60*60;
        
        if( $account->level >= config::MODERATOR_USER_LEVEL || $account->has_admin_rights_to_module("gallery") ) return;
        if( $item->creation_date > date("Y-m-d H:i:s", strtotime("$item->creation_date + $mem_ttl seconds")) ) return;
        
        $item_author = $item->get_author();
        $type        = trim(current($current_module->language->xpath("//media_types/media[@type='{$item->type}']/caption")));
        
        $subject = replace_escaped_vars(
            $current_module->language->email_templates->media_item_submitted->subject,
            array(
                '{$website_name}',
                '{$type}',
                '{$item_author}',
                '{$title}',
            ),
            array(
                $settings->get("engine.website_name"),
                $type,
                $item_author->display_name,
                $item->title,
            )
        );
        
        $user_ip  = get_user_ip(); $parts = explode(".", $user_ip); array_pop($parts);
        $segment  = implode(".", $parts);
        $boundary = date("Y-m-d H:i:s", strtotime("now - 7 days"));
        $where = array(
            "status = 'published'",
            "visibility = 'public'",
            "publishing_date >= '$boundary'",
            "creation_ip like '{$segment}.%'",
            "id_media <> '$item->id_media'",
        );
        $other_from_segment = $repository->find($where, 12, 0, "publishing_date desc");
        if( count($other_from_segment) == 0 )
        {
            $other_from_segment = "<p>{$current_module->language->email_templates->media_item_submitted->none_found}</p>";
        }
        else
        {
            $lis = "";
            foreach($other_from_segment as $other_item)
            {
                $published = time_mini_string($other_item->publishing_date);
                $link      = $other_item->get_page_url(true);
                $lis      .= "
                    <span style='display: inline-block; width: 150px; vertical-align: top; margin: 10px;'>
                        <a href='{$link}'><img width='150' border='1' src='{$other_item->get_thumbnail_url(true)}'></a><br>
                        <a href='{$config->full_root_url}/user/{$other_item->author_user_name}'>{$other_item->author_display_name}</a><br>
                        [$published • {$other_item->creation_ip}]:<br>
                        <a href='{$link}'>{$other_item->title}</a>
                    </span>
                ";
            }
            $other_from_segment = $lis;
        }
        
        $body = replace_escaped_vars(
            $current_module->language->email_templates->media_item_submitted->body,
            array(
                '{$item_author}',
                '{$type}',
                '{$thumbnail}',
                '{$title}',
                '{$description}',
                '{$ip}',
                '{$location}',
                '{$user_agent}',
                '{$other_from_segment}',
                '{$item_url}',
                '{$edit_url}',
                '{$preferences}',
                '{$website_name}',
            ),
            array(
                "<a href='{$config->full_root_url}/user/{$item_author->user_name}'>$item_author->display_name</a>",
                $type,
                $item->get_thumbnail_url(true),
                $item->title,
                $item->description,
                $user_ip,
                forge_geoip_location($user_ip),
                $_SERVER["HTTP_USER_AGENT"],
                $other_from_segment,
                $item->get_page_url(true),
                "{$config->full_root_url}/gallery/?edit_item={$item->id_media}&wasuuup=" . md5(mt_rand(1, 65535)),
                "{$config->full_root_url}/accounts/preferences.php",
                $settings->get("engine.website_name"),
            )
        );
        
        $body = unindent($body);
        broadcast_mail_to_moderators(
            $subject, $body, "@gallery:moderator_emails_for_media", array($item_author->id_account)
        );
    }
}
