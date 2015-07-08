#################################################################################
#
# $Date:: 2015-06-08 15:36:36 #$
# $Rev: 742 $
# $Author: komatsu2055 $
#
#################################################################################

Frontends::Application.routes.draw do
  # The priority is based upon order of creation: first created -> highest priority.
  # See how all your routes lay out with "rake routes".

  # You can have the root of your site routed with "root"
  root 'welcome#index'

  # Example of regular route:
  #   get 'products/:id' => 'catalog#view'

  # Example of named route that can be invoked with purchase_url(id: product.id)
  #   get 'products/:id/purchase' => 'catalog#purchase', as: :purchase

  # Example resource route (maps HTTP verbs to controller actions automatically):
  #   resources :products
  resources :sessions
  resources :jobunits do
    resources :children
    resources :rootjobnets
    resources :schedules
    resources :connectors
    resources :sleepjobs
    resources :clockjobs
    resources :datejobs
    resources :dateconds
    resources :vardata
    resources :sshjobs
    resources :winjobs
    resources :conditions
  end

  resources :proc_topjobnets do
    collection do
      get 'summarize'
    end
  end

  resources :proc_jobunits

  resources :hist_jobunits
  resources :children

  resources :calendars do
    resources :calendar_dates
  end
  resources :users
  resources :configures

  # Example resource route with options:
  #   resources :products do
  #     member do
  #       get 'short'
  #       post 'toggle'
  #     end
  #
  #     collection do
  #       get 'sold'
  #     end
  #   end

  # Example resource route with sub-resources:
  #   resources :products do
  #     resources :comments, :sales
  #     resource :seller
  #   end

  # Example resource route with more complex sub-resources:
  #   resources :products do
  #     resources :comments
  #     resources :sales do
  #       get 'recent', on: :collection
  #     end
  #   end

  # Example resource route with concerns:
  #   concern :toggleable do
  #     post 'toggle'
  #   end
  #   resources :posts, concerns: :toggleable
  #   resources :photos, concerns: :toggleable

  # Example resource route within a namespace:
  #   namespace :admin do
  #     # Directs /admin/products/* to Admin::ProductsController
  #     # (app/controllers/admin/products_controller.rb)
  #     resources :products
  #   end
end
