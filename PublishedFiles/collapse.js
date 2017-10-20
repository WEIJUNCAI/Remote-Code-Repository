function collapse(body, btn)
{
    var bodyToCollapse = document.getElementById(body);
    var buttonToModify = document.getElementById(btn);
    if (bodyToCollapse.style.display === 'none')
    {
        bodyToCollapse.style.display = 'inline';
        buttonToModify.innerHTML= "-";
    }
    else
    {
        bodyToCollapse.style.display = 'none';
        buttonToModify.innerHTML= "+";
    }
}