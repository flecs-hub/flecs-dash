Vue.component('sidebar', {
  props: {
    app: String
  },
  methods: {
    select_app(event) {
      this.$emit("app-select", event);
    }
  },
  template: `
    <div class="sidebar">
      <sidebar-button :app="'overview'" :active_app="app"
        v-on:select-app="select_app">
      </sidebar-button>

      <sidebar-button :app="'browser'" :active_app="app"
        v-on:select-app="select_app">
      </sidebar-button>
    </div>
    `
});
  