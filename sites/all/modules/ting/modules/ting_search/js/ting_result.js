// $Id$

/**
 * Search result handling function.
 */
Drupal.tingResult = function (searchResultElement, facetBrowserElement, result) {
  this.searchResultElement = searchResultElement;
  this.facetBrowserElement = facetBrowserElement;

  this.renderTingSearchResults = function (element, result) {
    $(element).find('ul,ol').html(result.result_html);
    Drupal.tingSearch.updateSummary($('#ting-search-summary'), result);
  };

  this.renderTingSearchResultPager = function (element, result) {
    var anchorVars, morePages, currentPage, $pager, pageNumberClasses;
    anchorVars = Drupal.getAnchorVars();
    morePages = (result.collectionCount >= result.resultsPerPage);
    currentPage = (anchorVars.page !== null) ? parseInt(anchorVars.page) : 1;

    // Don't bother with a pager if there is nothing to paginate to.
    if (morePages || currentPage > 1) {
      $pager = $(Drupal.settings.tingResult.pagerTemplate);

      // If we're on the first page, remove the previous  and first page
      // links from the template.
      if (currentPage < 2) {
        $pager.find('a.prev').parent().remove();
        $pager.find('a.first').parent().remove();
      }

      // If there's no more pages, remove the next link.
      if (!morePages) {
        $pager.find('a.next').parent().remove();
      }

      pageNumberClasses = {
        '.first': 1,
        '.prev': currentPage - 1,
        '.next': currentPage + 1
      };

      $.each(pageNumberClasses, function(i, e) {
        var page = pageNumberClasses[i];
        $pager.find(i).click(function() {
          Drupal.updatePageUrl(page);
          Drupal.doUrlSearch(facetBrowserElement, searchResultElement);
          return false;
        });
      });

      $($pager).find('ul a').click(function() {
        Drupal.updatePageUrl($(this).text());
        Drupal.doUrlSearch(facetBrowserElement, searchResultElement);
        return false;
      });

      // Kasper: What is the purpose of this?
      element = $(element);
      if (element.next().size() > 0) {
        element.next().replaceWith($pager);
      }
      else {
        element.after($pager);
      }
    }
  };

  this.doUrlSearch = function() {
      //Start loading
      Drupal.tingSearch.tabLoading('ting');

      var vars = Drupal.getAnchorVars();
      vars.query = Drupal.settings.tingSearch.keys;

      $.getJSON(Drupal.settings.tingSearch.ting_url, vars, function(data) {
        //Update tabs now that we have the result
        Drupal.tingSearch.summary.ting = { count: data.count, page: data.page };
        Drupal.tingSearch.updateTabs('ting');

        //Update search result and facet browser
        Drupal.renderTingSearchResults(Drupal.searchResultElement, data);
        Drupal.renderTingSearchResultPager(Drupal.searchResultElement, data);
        Drupal.updateFacetBrowser(Drupal.facetBrowserElement, data);
        Drupal.bindSelectEvent(Drupal.facetBrowserElement, searchResultElement);
        Drupal.updateSelectedFacetsFromUrl(Drupal.facetBrowserElement);
      });
  };

  this.updatePageUrl = function(pageNumber) {
    var anchorVars = Drupal.getAnchorVars();
    anchorVars.page = pageNumber;
    Drupal.setAnchorVars(anchorVars);
  };

  this.updateSortUrl = function(sort) {
    var anchorVars = Drupal.getAnchorVars();
    anchorVars.sort = sort;
    delete anchorVars.page;
    Drupal.setAnchorVars(anchorVars);
  };

  this.renderTingSearchResults(searchResultElement, result);
  this.renderTingSearchResultPager(searchResultElement, result);

  $('#edit-ting-search-sort').val(Drupal.getAnchorVars().sort);
  $('#edit-ting-search-sort').change(function() {
    Drupal.updateSortUrl($(this).val());
    Drupal.doUrlSearch(facetBrowserElement, searchResultElement);
  });
};

