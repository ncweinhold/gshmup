#include "entity.h"
#include "string.h"

static GshmupEntity *current_entity = NULL;

GshmupEntity
gshmup_create_entity (const gchar *name)
{
    GshmupEntity entity;

    entity.name = g_strdup (name);
    entity.position = gshmup_create_vector2 (0, 0);
    entity.hitbox = gshmup_create_rect (0, 0, 0, 0);
    entity.sprite_sheet = NULL;
    entity.agenda = gshmup_create_agenda ();

    return entity;
}

void
gshmup_draw_entity (GshmupEntity *entity)
{
    entity->sprite.position = entity->position;
    gshmup_draw_sprite (&entity->sprite);
}

void
gshmup_update_entity (GshmupEntity *entity)
{
    current_entity = entity;
    gshmup_update_agenda (entity->agenda);
}

/* Guile bindings! */
SCM_DEFINE (entity_position, "entity-position", 0, 0, 0,
            (void),
            "Return the position of the current entity.")
{
    return gshmup_scm_from_vector2 (current_entity->position);
}

SCM_DEFINE (move_entity, "move-entity", 1, 0, 0,
            (SCM v),
            "Add @var{v} to the currrent entity's position.")
{
    current_entity->position = gshmup_vector2_add (current_entity->position,
                                                   gshmup_scm_to_vector2 (v));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE (set_animation, "set-animation", 1, 0, 0,
            (SCM key),
            "Changes the current entity's sprite animation")
{
    gchar *anim_key;
    GshmupAnimation *anim;

    anim_key = scm_to_latin1_string (key);
    scm_dynwind_unwind_handler (g_free, anim_key, SCM_F_WIND_EXPLICITLY);
    anim = gshmup_get_sprite_sheet_animation (current_entity->sprite_sheet, anim_key);
    gshmup_set_sprite_animation (&current_entity->sprite, anim);

    return SCM_UNSPECIFIED;
}

void
gshmup_entity_init_scm (void)
{
#include "entity.x"

    scm_c_export (s_entity_position,
                  s_move_entity,
                  s_set_animation,
                  NULL);
}
